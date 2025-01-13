
#include "t27_forward.h"

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

#define PACKED_ELEMS ((FILTER_SIZE * INPUT_CH) + (4 - (FILTER_SIZE * INPUT_CH) % 4)) // 64 (= 9 * 7 + 1)

#define PACKED_FILTER_ROWS (PACKED_ELEMS / 4)
_Static_assert(PACKED_FILTER_ROWS <= 16, "AMX tile rows must be <= 16");

#define PACKED_FILTER_COLS (FILTER_CH * 4)
_Static_assert(PACKED_FILTER_COLS <= 64, "AMX tile rows must be <= 64");

DEFINE_BYTE_MATRIX(packed_filter_t, PACKED_FILTER_ROWS, PACKED_FILTER_COLS)

#define AMX_ROWS_16 16

static void init_tile_config() {
    tile_config_t tile = {0};
    tile.palette_id = 1;
    tile.start_row = 0;

    tile.colsb[0] = PACKED_FILTER_COLS * sizeof(int8_t);
    tile.rows[0] = PACKED_FILTER_ROWS;

    tile.colsb[1] = FILTER_CH * sizeof(int32_t);
    tile.rows[1] = AMX_ROWS_16;

    tile.colsb[2] = PACKED_ELEMS * sizeof(int8_t);
    tile.rows[2] = AMX_ROWS_16;

    tile.colsb[3] = FILTER_CH * sizeof(int32_t);
    tile.rows[3] = AMX_ROWS_16;

    tile.colsb[4] = PACKED_ELEMS * sizeof(int8_t);
    tile.rows[4] = AMX_ROWS_16;

    tile.colsb[5] = FILTER_CH * sizeof(int32_t);
    tile.rows[5] = AMX_ROWS_16;

    tile.colsb[6] = PACKED_ELEMS * sizeof(int8_t);
    tile.rows[6] = AMX_ROWS_16;

    // for (int i = 0; i < 8; i++) printf("tile[%d] = [rows: %d, colsb: %d]\n", i, tile.rows[i], tile.colsb[i]);

    _tile_loadconfig(&tile);
}

void store_packed_filter(packed_filter_t packed_filter[FILTER_SIZE], const filter7x7_t filter[INPUT_CH]) {
    memset(packed_filter, 0, sizeof(packed_filter_t) * FILTER_SIZE);

    for (int r = 0; r < FILTER_SIZE; ++r) {
        for (int c = 0; c < FILTER_SIZE; ++c) {
            for (int n = 0; n < FILTER_CH; ++n) {
                for (int ich = 0; ich < INPUT_CH; ++ich) {

                    const int c2 = n;
                    const int r2 = c * INPUT_CH + ich;

                    packed_filter[r].rows[r2 / 4].cols[c2 * 4 + r2 % 4] = (filter[ich].rows[r].cols[c].ch[n]);
                }
            }
        }
    }
}

void convolution(
        output_mat_t *restrict output,
        const input_mat_t *restrict input,
        const packed_filter_t packed_filter[FILTER_SIZE]) {
    for (int r = 0; r < OUTPUT_ROWS - FILTER_PADDING * 2; ++r) {
        for (int c = 0; c < OUTPUT_COLS - FILTER_PADDING * 2 - AMX_ROWS_16; c += AMX_ROWS_16 * 3) {
            if (c > OUTPUT_COLS - FILTER_PADDING * 2 - AMX_ROWS_16 * 2) {
                _tile_zero(1);

                for (int acc = 0; acc < FILTER_SIZE; ++acc) {
                    _tile_loadd(0, packed_filter[acc].bytes, PACKED_FILTER_COLS * sizeof(int8_t));

                    _tile_loadd(2, &input->rows[r + acc].cols[c], INPUT_CH * sizeof(int8_t));
                    _tile_dpbssd(1, 2, 0);
                }

                _tile_stored(1, &output->rows[r].cols[c], FILTER_CH * sizeof(int32_t));
            } else if (c > OUTPUT_COLS - FILTER_PADDING * 2 - AMX_ROWS_16 * 3) {
                _tile_zero(1);
                _tile_zero(3);

                for (int acc = 0; acc < FILTER_SIZE; ++acc) {
                    _tile_loadd(0, packed_filter[acc].bytes, PACKED_FILTER_COLS * sizeof(int8_t));

                    _tile_loadd(2, &input->rows[r + acc].cols[c], INPUT_CH * sizeof(int8_t));
                    _tile_dpbssd(1, 2, 0);

                    _tile_loadd(4, &input->rows[r + acc].cols[c + AMX_ROWS_16], INPUT_CH * sizeof(int8_t));
                    _tile_dpbssd(3, 4, 0);
                }

                _tile_stored(1, &output->rows[r].cols[c], FILTER_CH * sizeof(int32_t));
                _tile_stored(3, &output->rows[r].cols[c + AMX_ROWS_16], FILTER_CH * sizeof(int32_t));
            } else {
                _tile_zero(1);
                _tile_zero(3);
                _tile_zero(5);

                for (int acc = 0; acc < FILTER_SIZE; ++acc) {
                    _tile_loadd(0, packed_filter[acc].bytes, PACKED_FILTER_COLS * sizeof(int8_t));

                    _tile_loadd(2, &input->rows[r + acc].cols[c], INPUT_CH * sizeof(int8_t));
                    _tile_dpbssd(1, 2, 0);

                    _tile_loadd(4, &input->rows[r + acc].cols[c + AMX_ROWS_16], INPUT_CH * sizeof(int8_t));
                    _tile_dpbssd(3, 4, 0);

                    _tile_loadd(5, &input->rows[r + acc].cols[c + AMX_ROWS_16 * 2], INPUT_CH * sizeof(int8_t));
                    _tile_dpbssd(5, 6, 0);
                }

                _tile_stored(1, &output->rows[r].cols[c], FILTER_CH * sizeof(int32_t));
                _tile_stored(3, &output->rows[r].cols[c + AMX_ROWS_16], FILTER_CH * sizeof(int32_t));
                _tile_stored(5, &output->rows[r].cols[c + AMX_ROWS_16 * 2], FILTER_CH * sizeof(int32_t));
            }
        }

        {
            const int c = OUTPUT_COLS - FILTER_PADDING * 2 - AMX_ROWS_16;
            _tile_zero(1);

            for (int acc = 0; acc < FILTER_SIZE; ++acc) {
                _tile_loadd(0, packed_filter[acc].bytes, PACKED_FILTER_COLS * sizeof(int8_t));
                _tile_loadd(2, &input->rows[r + acc].cols[c], INPUT_CH * sizeof(int8_t));

                _tile_dpbssd(1, 2, 0);
            }

            _tile_stored(1, &output->rows[r].cols[c], FILTER_CH * sizeof(int32_t));
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
    init_input_mat(&input);

    // fprint_input_mat_t_to_file("out/a.txt", &input);

    output_mat_t output;

    filter7x7_t f[INPUT_CH];
    init_filter(f);

    // -----------------------------------------------

    if (!syscall_for_amx()) return 1;
    init_tile_config();

    packed_filter_t packed_filter[FILTER_SIZE];
    store_packed_filter(packed_filter, f);

    for (int i = 0; i < CONVOLUTION_COUNT; i++) {
        memset(&output, 0, sizeof(output_mat_t));

        convolution(&output, &input, packed_filter);

        for (int j = 0; j < FILTER_CH; ++j) input.bytes[j % INPUT_CH] += (int8_t) (output.rows[0].cols[0].ch[j]);
    }

    _tile_release();

    // -----------------------------------------------

    fprint_output_mat_t_to_file("out/t26_amx_o4_output.txt", &output);
}
