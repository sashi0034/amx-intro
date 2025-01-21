
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

#define PATCH_STRIDE_16 16
#define PATCH_INPUT_ROWS PATCH_STRIDE_16
#define PATCH_INPUT_COLS 10 // = 9 + 1

#define PATCH_FILTER_ROWS (PATCH_INPUT_COLS / 2)
#define PATCH_FILTER_COLS 2

#define PATCH_OUTPUT_ROWS PATCH_INPUT_ROWS
#define PATCH_OUTPUT_COLS 1

DEFINE_BF16MAT(PatchInputMat, PATCH_INPUT_ROWS, PATCH_INPUT_COLS)

DEFINE_BF16MAT(PatchFilterMat, PATCH_FILTER_ROWS, PATCH_FILTER_COLS)

DEFINE_MATRIX(PatchOutputMat,

              PATCH_OUTPUT_ROWS, PATCH_OUTPUT_COLS)

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

    // Result Register
    tile.colsb[PATCH_OUTPUT_REG_1] = PATCH_OUTPUT_COLS * sizeof(fp32_t);
    tile.rows[PATCH_OUTPUT_REG_1] = PATCH_OUTPUT_ROWS;

    // Source Register A
    tile.colsb[PATCH_INPUT_REG_2] = PATCH_INPUT_COLS * sizeof(bf16_t);
    tile.rows[PATCH_INPUT_REG_2] = PATCH_INPUT_ROWS;

    // Source Register B
    tile.colsb[PATCH_FILTER_REG_3] = PATCH_FILTER_COLS * sizeof(bf16_t);
    tile.rows[PATCH_FILTER_REG_3] = PATCH_FILTER_ROWS;

//    printf("1: (%d, %d), 2: (%d, %d), 3: (%d, %d)\n",
//           tile.colsb[PATCH_OUTPUT_REG_1], tile.rows[PATCH_OUTPUT_REG_1],
//           tile.colsb[PATCH_INPUT_REG_2], tile.rows[PATCH_INPUT_REG_2],
//           tile.colsb[PATCH_FILTER_REG_3], tile.rows[PATCH_FILTER_REG_3]);
//    fflush(stdout);

    _tile_loadconfig(&tile);
}

void load_patch_filter(const Filter3x3 *filter) {
    PatchFilterMat patch_filter;
    for (int r = 0; r < FILTER_SIZE; ++r) {
        for (int c = 0; c < FILTER_SIZE; ++c) {
            const int index = r * FILTER_SIZE + c;
            patch_filter.rows[index / 2].cols[index % 2] = fp32_to_bf16(filter->rows[r].cols[c]);
        }
    }

    _tile_loadd(PATCH_FILTER_REG_3, patch_filter.bf16s, PATCH_FILTER_COLS * sizeof(bf16_t));
}

void load_patch_input(PatchInputMat *patch_input, const SimMat *input, int patchRaw, int col16) {
    for (int pr = 0; pr < PATCH_INPUT_ROWS; ++pr) {
        for (int pc = 0; pc < FILTER_SIZE * FILTER_SIZE; ++pc) {
            if (col16 * PATCH_STRIDE_16 + pr >= SIM_MAT_COLS - FILTER_PADDING * 2) {
                // フィルタを適応しない部分
                patch_input->rows[pr].cols[pc] = 0;
                continue;
            }

            const int offsetR = patchRaw + (pc / FILTER_SIZE);
            const int offsetC = col16 * PATCH_STRIDE_16 + pr + (pc % FILTER_SIZE);

            patch_input->rows[pr].cols[pc] = fp32_to_bf16(input->rows[offsetR].cols[offsetC]);
        }

        for (int pc = FILTER_SIZE * FILTER_SIZE; pc < PATCH_INPUT_COLS; ++pc) {
            patch_input->rows[pr].cols[pc] = 0;
        }
    }

    _tile_loadd(PATCH_INPUT_REG_2, patch_input->bf16s, PATCH_INPUT_COLS * sizeof(bf16_t));
}

static void store_patch_output(SimMat *output, int patchRaw, int col16) {
    _tile_stored(PATCH_OUTPUT_REG_1,
                 output->fp32s + col16 * PATCH_STRIDE_16 + (patchRaw) * SIM_MAT_COLS,
                 PATCH_OUTPUT_COLS * sizeof(fp32_t)
    );

    __asm__ __volatile__ ("" : "+m" (output->fp32s));
}

void convolution_amx(SimMat *output, const SimMat *input, const Filter3x3 *filter) {
    PatchInputMat patch_input;
    PatchOutputMat patch_output;

    memset(&patch_output, 0, sizeof(PatchOutputMat));

    for (int patchRaw = 0; patchRaw < SIM_MAT_ROWS - FILTER_PADDING * 2; ++patchRaw) {
        for (int col16 = 0; col16 < SIM_MAT_COLS / PATCH_STRIDE_16; ++col16) {
            _tile_loadd(PATCH_OUTPUT_REG_1, patch_output.fp32s, PATCH_OUTPUT_COLS * sizeof(fp32_t));

            load_patch_input(&patch_input, input, patchRaw, col16);

            _tile_dpbf16ps(PATCH_OUTPUT_REG_1, PATCH_INPUT_REG_2, PATCH_FILTER_REG_3);

            store_patch_output(output, patchRaw, col16);
        }
    }
}

// -----------------------------------------------

static void mock_task(SimMat *output, float f[restrict NB][NZ2][NY2][NX2], const Filter3x3 *filter) {
    SimMat *input = (SimMat *) (f[SAMPLE_LAYER_N][SAMPLE_LAYER_Z]);

    convolution_amx(output, input, filter);
}

// -----------------------------------------------

int main() {
    SimState simState;
    init_sim_state(&simState);

    prepare_system_for_amx();

    init_tile_config();

    Filter3x3 filter;
    make_filter(&filter);
    load_patch_filter(&filter);

    ConvOutput output;
    memset(&output, 0, sizeof(ConvOutput));

    // Main loop
    for (int ii = 1; ii <= LAST; ii++) {
        tick_sim_state(&simState, ii);

        mock_task(&output.mats[ii - 1], simState.f, &filter);
    }

    output_conv(&output, "output/out_amx.txt");

    _tile_release();
    return 0;
}
