
#include "t10_mhd.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdbool.h>
#include <immintrin.h>

#define NOUT0 64

#define DEFINE_MATRIX(name, r, c) \
    typedef struct name { \
        union { \
            float fp32s[(r) * (c)]; \
            struct { \
                float cols[(c)]; \
            } rows[(r)]; \
        }; \
    } name; \
    \
    void print_##name(const name* mat) { \
        for (int i = 0; i < (r); ++i) { \
            for (int j = 0; j < (c); ++j) { \
                printf("%f ", (mat->rows[i].cols[j])); \
            } \
            printf("\n"); \
        } \
    } \
    \
    void fprint_##name(FILE* file, const name* mat) { \
        for (int i = 0; i < (r); ++i) { \
            for (int j = 0; j < (c); ++j) { \
                fprintf(file, "%f ", (mat->rows[i].cols[j])); \
            } \
            fprintf(file, "\n"); \
        } \
    } \
    \
    void fprint_##name##_to_file(const char* filename, const name* mat) { \
        FILE* file = fopen(filename, "w"); \
        if (!file) { \
            perror("Failed to open file"); \
            return; \
        } \
        fprint_##name(file, mat); \
        fclose(file); \
    }

#define MATRIX_ROWS 192
#define MATRIX_COLS 192

DEFINE_MATRIX(Matrix, MATRIX_ROWS, MATRIX_COLS)

#define FILTER_SIZE 3
#define FILTER_OFFSET ((FILTER_SIZE - 1) / 2)

DEFINE_MATRIX(Filter3x3, FILTER_SIZE, FILTER_SIZE)

// -----------------------------------------------
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

DEFINE_MATRIX(PatchOutputMat, PATCH_OUTPUT_ROWS, PATCH_OUTPUT_COLS)

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

void load_patch_filter(PatchFilterMat *patch_filter, const Filter3x3 *filter) {
    for (int r = 0; r < FILTER_SIZE; ++r) {
        for (int c = 0; c < FILTER_SIZE; ++c) {
            const int index = r * FILTER_SIZE + c;
            patch_filter->rows[index / 2].cols[index % 2] = fp32_to_bf16(filter->rows[r].cols[c]);
        }
    }

    _tile_loadd(PATCH_FILTER_REG_3, patch_filter->bf16s, PATCH_FILTER_COLS * sizeof(bf16_t));
}

void load_patch_input(PatchInputMat *patch_input, const Matrix *input, int patchRaw, int col16) {
    for (int pr = 0; pr < PATCH_INPUT_ROWS; ++pr) {
        for (int pc = 0; pc < PATCH_INPUT_COLS; ++pc) {
            if (pc >= FILTER_SIZE * FILTER_SIZE) {
                // フィルタが存在しない部分
                patch_input->rows[pr].cols[pc] = 0;
                continue;
            }

            if (col16 * PATCH_STRIDE_16 + pr >= MATRIX_COLS - FILTER_OFFSET * 2) {
                // フィルタを適応しない部分
                patch_input->rows[pr].cols[pc] = 0;
                continue;
            }

            const int offsetR = patchRaw + (pc / FILTER_SIZE);
            const int offsetC = col16 * PATCH_STRIDE_16 + pr + (pc % FILTER_SIZE);

            patch_input->rows[pr].cols[pc] = fp32_to_bf16(input->rows[offsetR].cols[offsetC]);
        }
    }

    _tile_loadd(PATCH_INPUT_REG_2, patch_input->bf16s, PATCH_INPUT_COLS * sizeof(bf16_t));
}

static void store_patch_output(Matrix *output, int patchRaw, int col16) {
    _tile_stored(PATCH_OUTPUT_REG_1,
                 output->fp32s + col16 * PATCH_STRIDE_16 + (patchRaw) * MATRIX_COLS,
                 PATCH_OUTPUT_COLS * sizeof(fp32_t)
    );

    __asm__ __volatile__ ("" : "+m" (output->fp32s));
}

static void init_patch_output(PatchOutputMat *patch_output) {
    memset(patch_output, 0, sizeof(PatchOutputMat));
}

void convolution_amx(Matrix *output, const Matrix *input, const Filter3x3 *filter) {
    PatchInputMat patch_input;
    PatchFilterMat patch_filter;
    PatchOutputMat patch_output;

    load_patch_filter(&patch_filter, filter);

    init_patch_output(&patch_output);

    for (int patchRaw = 0; patchRaw < MATRIX_ROWS - FILTER_OFFSET * 2; ++patchRaw) {
        for (int col16 = 0; col16 < MATRIX_COLS / PATCH_STRIDE_16; ++col16) {
            _tile_loadd(PATCH_OUTPUT_REG_1, patch_output.fp32s, PATCH_OUTPUT_COLS * sizeof(fp32_t));

            load_patch_input(&patch_input, input, patchRaw, col16);

            _tile_dpbf16ps(PATCH_OUTPUT_REG_1, PATCH_INPUT_REG_2, PATCH_FILTER_REG_3);

            store_patch_output(output, patchRaw, col16);
        }
    }
}

// -----------------------------------------------

#define SAMPLE_LAYER_N 6
#define SAMPLE_LAYER_Z (NZ2 / 2)

void print_sample_layer(float f[restrict NB][NZ2][NY2][NX2]) {
    for (int y = 0; y < NY2; y++) {
        for (int x = 0; x < NX2; x++) {
            if (f[SAMPLE_LAYER_N][SAMPLE_LAYER_Z][y][x] != 0) {
                printf("%f ", 1000 * f[SAMPLE_LAYER_N][SAMPLE_LAYER_Z][y][x]);
            }
        }
        printf("\n");
    }
}

// -----------------------------------------------



void make_filter(Filter3x3 *filter) {
    for (int r = 0; r < FILTER_SIZE; ++r) {
        for (int c = 0; c < FILTER_SIZE; ++c) {
            filter->rows[r].cols[c] = (float) ((r + c) % 2);
        }
    }
}

static void mock_task(float f[restrict NB][NZ2][NY2][NX2], Filter3x3 *filter) {
    Matrix *input = (Matrix *) (f[SAMPLE_LAYER_N][SAMPLE_LAYER_Z]);
//    Matrix input;
//    for (int r = 0; r < MATRIX_ROWS; ++r) {
//        for (int c = 0; c < MATRIX_COLS; ++c) {
//            input.rows[r].cols[c] = f[SAMPLE_LAYER_N][SAMPLE_LAYER_Z][r][c];
//        }
//    }

    Matrix output;
    memset(&output, 0, sizeof(output));

    convolution_amx(&output, input, filter);

    fprint_Matrix_to_file("output/out_amx.txt", &output);
}


// -----------------------------------------------

int main() {
    // Time variables
    clock_t zt0, zt1, zt2, zt3;
    double zt, ztt;

    // Simulation arrays
    float (*restrict f)[NZ2][NY2][NX2] = (float (*)[NZ2][NY2][NX2]) malloc(NB * NZ2 * NY2 * NX2 * sizeof(float));
    float (*restrict u)[NZ2][NY2][NX2] = (float (*)[NZ2][NY2][NX2]) malloc(NB * NZ2 * NY2 * NX2 * sizeof(float));
    float (*restrict pp)[NZ2][NY2][NX2] = (float (*)[NZ2][NY2][NX2]) malloc(NBBB * NZ2 * NY2 * NX2 * sizeof(float));

    int nmlf = 0, nupb = 0, nout = 0, ii;
    int ntap = 10;

    float vmax = 0.0;

    // Simulation information
    simuinfo();

    // Record start time
    zt0 = clock();

    // Initial conditions
    equil_out(f, pp);

    // Record initial time
    zt1 = clock();

    prepare_system_for_amx();

    init_tile_config();

    Filter3x3 filter;
    make_filter(&filter);

    // Main loop
    for (ii = 1; ii <= LAST; ii++) {
        nmlf++;
        nupb++;
        nout++;

        // Outer boundary for f
        bdry_z_m(f, u);

        // Main calculation
        mlf(f, u, pp, ii, nmlf, vmax);

        if (vmax > 1.0)
            nout = NOUT0;

        if (nmlf == NMLF0)
            nmlf = 0;

        // Upward boundary condition for f
        if (nupb == NUPB0) {
            nupb = 0;
            bdry_up(f, u);
        }

        // -----------------------------------------------

        if (ii == 2) {
            printf("----------------------------------------------- %d\n", ii);
            // print_sample_layer(f);
            mock_task(f, &filter);
            break;
        }

        // -----------------------------------------------

        // Finalization part (write data, etc.)
        if (nout == NOUT0) {
            nout = 0;

            // Record end time
            zt3 = clock();

            // Output information
            ntap++;

            printf("    ii, LAST, ntap,   vmax,    THX\n");
            printf("%6d%6d%6d%8.4f%8.4f\n", ii, LAST, ntap, vmax, THX);
            printf(" \n");

            zt = (double) (zt3 - zt1) / CLOCKS_PER_SEC;
            ztt = (double) (zt1 - zt0) / CLOCKS_PER_SEC;

            printf("    ii,      init,        zt\n");
            printf("%6d%11.4f%11.4f\n", ii, ztt, zt);
            printf(" \n");

            gflops(zt);
        }
    }

    _tile_release();
    return 0;
}
