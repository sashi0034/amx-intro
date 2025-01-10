
#include "t26_forward.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

// -----------------------------------------------
// https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#!=undefined&techs=AMX
typedef struct { // __tile_config
    uint8_t palette_id;
    uint8_t start_row;
    uint8_t reserved_0[14];
    uint16_t colsb[16];
    uint8_t rows[16];
} TileConfig;

#define ARCH_GET_XCOMP_PERM 0x1022
#define ARCH_REQ_XCOMP_PERM 0x1023
#define XFEATURE_XTILECFG 17
#define XFEATURE_XTILEDATA 18

#define PATCH_STRIDE_16 16
#define PATCH_INPUT_ROWS PATCH_STRIDE_16
#define PATCH_INPUT_COLS 52 // = 49 + 3

#define PATCH_FILTER_ROWS (PATCH_INPUT_COLS / 4)
#define PATCH_FILTER_COLS 4

#define PATCH_OUTPUT_ROWS PATCH_INPUT_ROWS
#define PATCH_OUTPUT_COLS 1

DEFINE_BYTE_MATRIX(path_input_t, PATCH_INPUT_ROWS, PATCH_INPUT_COLS) // 16x52

DEFINE_BYTE_MATRIX(path_filter_t, PATCH_FILTER_ROWS, PATCH_FILTER_COLS) // 13x4

DEFINE_DWORD_MATRIX(path_output_t, PATCH_OUTPUT_ROWS, PATCH_OUTPUT_COLS) // 16x1

static void init_tile_config() {
    TileConfig tile = {0};
    tile.palette_id = 1;
    tile.start_row = 0;

    tile.colsb[0] = PATCH_OUTPUT_COLS * sizeof(int32_t);
    tile.rows[0] = PATCH_OUTPUT_ROWS;

    tile.colsb[2] = PATCH_INPUT_COLS * sizeof(int8_t);
    tile.rows[2] = PATCH_INPUT_ROWS;

    tile.colsb[3] = PATCH_FILTER_COLS * sizeof(int8_t);
    tile.rows[3] = PATCH_FILTER_ROWS;

    _tile_loadconfig(&tile);
}

void load_patch_filter(const filter7x7_t *filter) {
    path_filter_t patch_filter;

    for (int r = 0; r < FILTER_SIZE; ++r) {
        for (int c = 0; c < FILTER_SIZE; ++c) {
            const int index = r * FILTER_SIZE + c;
            patch_filter.rows[index / 4].cols[index % 4] = (filter->rows[r].cols[c]);
        }
    }

    _tile_loadd(3, patch_filter.bytes, PATCH_FILTER_COLS * sizeof(uint8_t));
}

void convolution_amx(output_mat_t *restrict output, const input_mat_t *restrict input) {
    static path_input_t patch_input;
    static path_output_t patch_output;

    // memset(&patch_output, 0, sizeof(PatchOutputMat));

    for (int patch_raw = 0; patch_raw < INPUT_ROWS - FILTER_OFFSET * 2; ++patch_raw) {
        for (int col16 = 0; col16 < INPUT_COLS / PATCH_STRIDE_16; ++col16) {
            _tile_zero(0);

            const int pr_end = min(INPUT_ROWS - FILTER_OFFSET * 2 - col16 * PATCH_STRIDE_16, PATCH_INPUT_ROWS);
            for (int pr = 0; pr < pr_end; ++pr) {
                for (int pc = 0; pc < FILTER_SIZE * FILTER_SIZE; ++pc) {
                    const int offset_r = patch_raw + (pc / FILTER_SIZE);
                    const int offset_c = col16 * PATCH_STRIDE_16 + pr + (pc % FILTER_SIZE);

                    patch_input.rows[pr].cols[pc] = input->rows[offset_r].cols[offset_c];
                }
            }

            for (int pr = pr_end; pr < PATCH_INPUT_ROWS; ++pr) {
                for (int pc = 0; pc < PATCH_INPUT_COLS; ++pc) {
                    patch_input.rows[pr].cols[pc] = 0;
                }
            }

            // -----------------------------------------------

            _tile_loadd(2, patch_input.bytes, PATCH_INPUT_COLS * sizeof(uint8_t));

            _tile_dpbssd(0, 2, 3);

            _tile_stored(0,
                         output->dwords + col16 * PATCH_STRIDE_16 + patch_raw * OUTPUT_COLS,
                         PATCH_OUTPUT_COLS * sizeof(int32_t));
        }
    }
}

static bool syscall_for_amx() {
    if (syscall(SYS_arch_prctl, ARCH_REQ_XCOMP_PERM, XFEATURE_XTILEDATA)) {
        printf("\n Fail to do XFEATURE_XTILEDATA \n\n");
        fflush(stdout);
        return false;
    } else {
        return true;
    }
}

// -----------------------------------------------

int main() {
    input_mat_t input;
    for (int r = 0; r < INPUT_ROWS; ++r) {
        for (int c = 0; c < INPUT_COLS; ++c) {
            input.rows[r].cols[c] = (int8_t) (((r * INPUT_COLS + c) % 256) - 128);
        }
    }

    output_mat_t output;

    filter7x7_t f;
    for (int r = 0; r < FILTER_SIZE; ++r) {
        for (int c = 0; c < FILTER_SIZE; ++c) {
            f.rows[r].cols[c] = (int8_t) ((r + c) % 2);
        }
    }

    // -----------------------------------------------

    if (!syscall_for_amx()) return 1;
    init_tile_config();

    load_patch_filter(&f);

    for (int i = 0; i < CONVOLUTION_COUNT; i++) {
        memset(&output, 0, sizeof(output_mat_t));

        convolution_amx(&output, &input);

        f.bytes[0] = (int8_t) (output.rows[0].cols[0]);
    }

    _tile_release();

    // -----------------------------------------------

    fprint_output_mat_t_to_file("out/t26_amx_output.txt", &output);
}
