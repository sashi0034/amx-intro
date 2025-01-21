
#include "t30_mhd.h"
#include "t30_sim.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdbool.h>
#include <immintrin.h>

typedef uint16_t bf16_t;

typedef float fp32_t;

static bf16_t fp32_to_bf16(fp32_t value) {
    const uint16_t v = (*((uint32_t *) &value)) >> 16;
    return v;
}

static fp32_t bf16_to_fp32(bf16_t value) {
    const uint32_t v = ((uint32_t) value) << 16;
    return (*((fp32_t *) &v));
}

// https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#!=undefined&techs=AMX
typedef struct { // __tile_config
    uint8_t palette_id;
    uint8_t start_row;
    uint8_t reserved_0[14];
    uint16_t colsb[16];
    uint8_t rows[16];
} TileConfig;

#define ARCH_GET_XCOMP_PERM     0x1022
#define ARCH_REQ_XCOMP_PERM     0x1023
#define XFEATURE_XTILECFG       17
#define XFEATURE_XTILEDATA      18

#define DEFINE_BF16MAT(name, r, c) \
    typedef struct name { \
        union { \
            bf16_t bf16s[(r) * (c)]; \
            struct { \
                bf16_t cols[(c)]; \
            } rows[(r)]; \
        }; \
    } name; \
    \
    void print_##name(const name* mat) { \
        for (int i = 0; i < (r); ++i) { \
            for (int j = 0; j < (c); ++j) { \
                printf("%f ", bf16_to_fp32(mat->rows[i].cols[j])); \
            } \
            printf("\n"); \
        } \
    }

DEFINE_BF16MAT(tfilter_t, 2, 2);

DEFINE_BF16MAT(input_buffer_t, SIM_MAT_ROWS, SIM_MAT_COLS);

#define PATCH_STRIDE_16 16
#define PATCH_INPUT_ROWS PATCH_STRIDE_16
#define PATCH_INPUT_COLS 10 // = 9 + 1

#define PATCH_FILTER_ROWS (PATCH_INPUT_COLS / 2)
#define PATCH_FILTER_COLS 2

#define PATCH_OUTPUT_ROWS PATCH_INPUT_ROWS
#define PATCH_OUTPUT_COLS 1

static bool prepare_system_for_amx() {
    if (syscall(SYS_arch_prctl, ARCH_REQ_XCOMP_PERM, XFEATURE_XTILEDATA)) {
        printf("Failed to do XFEATURE_XTILEDATA\n");
        return false;
    } else {
        printf("Succeeded to do XFEATURE_XTILEDATA\n");
        return true;
    }
}

#define PATCH_OUTPUT_REG_1 1
#define PATCH_INPUT_REG_2 2
#define PATCH_FILTER_REG_3 3

static void init_tile_config() {
    TileConfig tile = {0};
    tile.palette_id = 1;
    tile.start_row = 0;

    // Filter
    tile.colsb[0] = 2 * sizeof(bf16_t);
    tile.rows[0] = 2;

    // Output 0
    tile.colsb[1] = 1 * sizeof(fp32_t);
    tile.rows[1] = 16;

    // Input 0
    tile.colsb[2] = 4 * sizeof(bf16_t);
    tile.rows[2] = 16;

//    // Output 1
//    tile.colsb[3] = 1 * sizeof(fp32_t);
//    tile.rows[3] = 16;
//
//    // Input 1
//    tile.colsb[4] = 4 * sizeof(bf16_t);
//    tile.rows[4] = 16;
//
//    // Output 2
//    tile.colsb[5] = 1 * sizeof(fp32_t);
//    tile.rows[5] = 16;
//
//    // Input 2
//    tile.colsb[6] = 4 * sizeof(bf16_t);
//    tile.rows[6] = 16;

    _tile_loadconfig(&tile);
}

void store_tfilter(tfilter_t tfilter[3], const Filter3x3 *filter) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            const int pc = 0;
            const int pr = c;
            tfilter[r].rows[pr / 2].cols[pc * 4 + pr % 2] = fp32_to_bf16(filter->rows[r].cols[c]);
        }

        tfilter[r].bf16s[3] = 0;
    }
}

void store_input_buffer(input_buffer_t *input_buffer, const SimMat *input) {
    for (int r = 0; r < SIM_MAT_ROWS; ++r) {
        _Static_assert(SIM_MAT_COLS % 32 == 0, "Cols must be multiple of 32");
        for (int c = 0; c < SIM_MAT_COLS; c += 32) {
            // 1. まず前半の 16 要素をロード
            __m512 zmm0 = _mm512_loadu_ps(&input->rows[r].cols[c]); // float 16要素
            // 2. 続いて後半の 16 要素をロード
            __m512 zmm1 = _mm512_loadu_ps(&input->rows[r].cols[c + 16]); // float 16要素

            // 3. 2 つの __m512 (計 32 要素) を bfloat16 に圧縮変換
            __m512bh bf16_32 = _mm512_cvtne2ps_pbh(zmm1, zmm0);

            // 4. 変換結果 (32 要素分の bf16 = 64 バイト) を out に書き込み
            _mm512_storeu_si512(&input_buffer->rows[r].cols[c], bf16_32);
        }
    }
}

void convolution_amx(SimMat *output, const input_buffer_t *input, const tfilter_t tfilter[3]) {
    for (int r = 0; r < SIM_MAT_ROWS - FILTER_PADDING * 2; ++r) {
        for (int c = 0; c < SIM_MAT_COLS - FILTER_PADDING * 2 - 16; c += 16) {
            _tile_zero(1);
            for (int acc = 0; acc < 3; ++acc) {
                _tile_loadd(0, tfilter[acc].bf16s, 2 * sizeof(bf16_t));
                _tile_loadd(2, &input->rows[r + acc].cols[c], 1 * sizeof(bf16_t));
                _tile_dpbf16ps(1, 2, 0);
            }

            _tile_stored(1, &output->rows[r].cols[c], 1 * sizeof(fp32_t));
        }

        {
            const int c = SIM_MAT_COLS - FILTER_PADDING * 2 - 16;
            _tile_zero(1);
            for (int acc = 0; acc < 3; ++acc) {
                _tile_loadd(0, tfilter[acc].bf16s, 2 * sizeof(bf16_t));
                _tile_loadd(2, &input->rows[r + acc].cols[c], 1 * sizeof(bf16_t));
                _tile_dpbf16ps(1, 2, 0);
            }

            _tile_stored(1, &output->rows[r].cols[c], 1 * sizeof(fp32_t));
        }
    }
}

// -----------------------------------------------

static void mock_task(SimMat *output, float f[restrict NB][NZ2][NY2][NX2], const tfilter_t tfilter[3]) {
    SimMat *input = (SimMat *) (f[SAMPLE_LAYER_N][SAMPLE_LAYER_Z]);

    static input_buffer_t input_buffer;
    store_input_buffer(&input_buffer, input);

    convolution_amx(output, &input_buffer, tfilter);
}

// -----------------------------------------------

int main() {
    SimState simState;
    init_sim_state(&simState);

    prepare_system_for_amx();
    init_tile_config();

    Filter3x3 filter;
    make_filter(&filter);

    tfilter_t tfilter[3];
    store_tfilter(tfilter, &filter);

    output_t output;
    memset(&output, 0, sizeof(output_t));

    printf("start main loop\n");
    fflush(stdout);

    // Main loop
    for (int ii = 1; ii <= LAST; ii++) {
        tick_sim_state(&simState, ii);

        mock_task(&output.mats[ii - 1], simState.f, tfilter);
    }

    output_conv(&output, "output/out_amx.txt");

    _tile_release();
    return 0;
}
