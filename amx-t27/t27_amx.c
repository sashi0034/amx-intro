
#include "t27_forward.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

#define TRACE() printf("[%s:%d] ", __FILE__, __LINE__); fflush(stdout);

// -----------------------------------------------
// https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#!=undefined&techs=AMX
typedef struct { // __tile_config
    uint8_t palette_id;
    uint8_t start_row;
    uint8_t reserved_0[14];
    uint16_t colsb[16];
    uint8_t rows[16];
} tile_config_t;

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

#define PACKED_ELEMS ((FILTER_SIZE * FILTER_SIZE) + (4 - FILTER_SIZE * FILTER_SIZE % 4)) // 52 (= 49 + 3)

#define PACKED_FILTER_ROWS (PACKED_ELEMS / 4)
_Static_assert(PACKED_FILTER_ROWS <= 16, "AMX tile rows must be <= 16");

#define PACKED_FILTER_COLS (FILTER_CH * 4)
_Static_assert(PACKED_FILTER_COLS <= 64, "AMX tile rows must be <= 64");

DEFINE_BYTE_MATRIX(packed_filter_t, PACKED_FILTER_ROWS, PACKED_FILTER_COLS) // 13x64

static void init_tile_config() {
    tile_config_t tile = {0};
    tile.palette_id = 1;
    tile.start_row = 0;

    tile.colsb[0] = FILTER_CH * sizeof(int32_t);
    tile.rows[0] = 1;

    tile.colsb[1] = PACKED_FILTER_COLS * sizeof(int8_t);
    tile.rows[1] = PACKED_FILTER_ROWS;

    tile.colsb[2] = PACKED_ELEMS * sizeof(int8_t);
    tile.rows[2] = 1;

    // for (int i = 0; i < 8; i++) printf("tile[%d] = [rows: %d, colsb: %d]\n", i, tile.rows[i], tile.colsb[i]);

    _tile_loadconfig(&tile);
}

void load_patch_filter(const filter7x7_t *filter) {
    packed_filter_t packed_filter;
    memccpy(&packed_filter, filter, sizeof(filter7x7_t), 0);

    for (int r = 0; r < FILTER_SIZE; ++r) {
        for (int c = 0; c < FILTER_SIZE; ++c) {
            for (int n = 0; n < FILTER_CH; ++n) {
                const int c2 = n;
                const int r2 = r * FILTER_SIZE + c;

                packed_filter.rows[r2 / 4].cols[c2 * 4 + r2 % 4] = (filter->rows[r].cols[c].ch[n]);
            }
        }
    }

    _tile_loadd(1, packed_filter.bytes, PACKED_FILTER_COLS * sizeof(int8_t));
}

void convolution_amx(output_mat_t *restrict output, const input_mat_t *restrict input) {
    for (int r = 0; r < OUTPUT_ROWS; ++r) {
        for (int c = 0; c < OUTPUT_COLS; ++c) {
            _tile_zero(0);

            _tile_loadd(2, &input->rows[r].cols[c], INPUT_COLS * sizeof(int8_t));

            _tile_dpbssd(0, 2, 1);

            _tile_stored(0, &output->rows[r].cols[c], sizeof(int32_t)); // FIXME: stride

            _tile_stored(0,
                         &output->rows[r].cols[c],
                         1 * sizeof(int32_t));
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
    init_filter(&f);

    // -----------------------------------------------

    if (!syscall_for_amx()) return 1;
    init_tile_config();

    load_patch_filter(&f);

    for (int i = 0; i < CONVOLUTION_COUNT; i++) {
        load_patch_filter(&f);
        memset(&output, 0, sizeof(output_mat_t));

        convolution_amx(&output, &input);

        for (int j = 0; j < FILTER_CH; ++j) input.bytes[0] += (int8_t) (output.rows[0].cols[0].ch[j]);
    }

    _tile_release();

    // -----------------------------------------------

    fprint_output_mat_t_to_file("out/t26_amx_output.txt", &output);
}
