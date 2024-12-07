
#include "t10_mhd.h"

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <openacc.h>

#define NOUT0 64

typedef float fp32_t;

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

#define DEFINE_MATRIX(name, r, c) \
    typedef struct name { \
        union { \
            float fp32s[(r) * (c)]; \
            struct { \
                float cols[(c)]; \
            } rows[(r)];          \
            float mat[r][c]; \
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

#define SIM_MAT_ROWS 192
#define SIM_MAT_COLS 192

DEFINE_MATRIX(SimMat, SIM_MAT_ROWS, SIM_MAT_COLS)

#define FILTER_SIZE 3
#define FILTER_PADDING ((FILTER_SIZE - 1) / 2)

DEFINE_MATRIX(Filter3x3, FILTER_SIZE, FILTER_SIZE)

static void convolution_gpu(
        float output[SIM_MAT_ROWS][SIM_MAT_COLS],
        const float input[SIM_MAT_ROWS][SIM_MAT_COLS],
        const float filter[FILTER_SIZE][FILTER_SIZE]) {
#pragma acc data copyin(input, filter) copyout(output)
    {
#pragma acc parallel loop collapse(2)
        for (int r = 0; r < SIM_MAT_ROWS - FILTER_PADDING * 2; r++) {
            for (int c = 0; c < SIM_MAT_COLS - FILTER_PADDING * 2; c++) {
                float sum = 0.0f;
                for (int fi = 0; fi < FILTER_SIZE; fi++) {
                    for (int fj = 0; fj < FILTER_SIZE; fj++) {
                        sum += input[r + fi][c + fj] * filter[fi][fj];
                    }
                }

                output[r][c] = sum;
            }
        }
    }
}

void make_filter(Filter3x3 *filter) {
    for (int r = 0; r < FILTER_SIZE; ++r) {
        for (int c = 0; c < FILTER_SIZE; ++c) {
            filter->rows[r].cols[c] = (float) ((r + c) % 2);
        }
    }
}

static void mock_task(const float f[restrict NB][NZ2][NY2][NX2], const Filter3x3 *filter) {
    SimMat output;
    memset(&output, 0, sizeof(output));

    convolution_gpu(output.mat, f[SAMPLE_LAYER_N][SAMPLE_LAYER_Z], filter->mat);

    fprint_SimMat_to_file("output/out_gpu.txt", &output);
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

    return 0;
}
