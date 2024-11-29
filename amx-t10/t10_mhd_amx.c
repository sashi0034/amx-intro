
#include "t10_mhd.h"
#include "t10_amx_8x16.h"

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define NOUT0 64

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

typedef struct FP32_8x8_24x24 {
    union {
        FP32_8x8 elem[192 / 8][192 / 8]; // 24x24
    };
} FP32_8x8_24x24;

typedef struct FP32_8x16_24x12 {
    union {
        FP32_8x16 elem[192 / 8][192 / 16]; // 24x12
    };
} FP32_8x16_24x12;

typedef struct FP32_16x8_12x24 {
    union {
        FP32_16x8 elem[192 / 16][192 / 8]; // 12x24
    };
} FP32_16x8_12x24;

static void dp_192x192(FP32_8x8_24x24 *c, const FP32_8x16_24x12 *a, const FP32_16x8_12x24 *b) {
    for (int i = 0; i < 192 / 8; i++) {
        for (int j = 0; j < 192 / 8; j++) {
            for (int k = 0; k < 192 / 16; k++) {
                amx_dp_8x16_16x8(&c->elem[i][j], &a->elem[i][k], &b->elem[k][j]);
            }
        }
    }
}

static void fprint_8x8_24x24(FP32_8x8_24x24 *c) {
    FILE *file = fopen("output/out_amx.txt", "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    for (int i = 0; i < 192 / 8; i++) {
        for (int y = 0; y < 8; y++) {
            for (int j = 0; j < 192 / 8; j++) {
                for (int x = 0; x < 8; x++) {
                    fprintf(file, "%f ", c->elem[i][j].rows[y].cols[x]);
                }
            }
            fprintf(file, "\n");
        }
    }

    fclose(file);
}

void make_filter(FP32_16x8_12x24 *filter) {
    for (int y1 = 0; y1 < 12; y1++) {
        for (int x1 = 0; x1 < 24; x1++) {
            for (int y0 = 0; y0 < 16; y0++) {
                for (int x0 = 0; x0 < 8; x0++) {
                    filter->elem[y1][x1].rows[y0].cols[x0] = (fp32_t) ((x0 + y0) % 2);
                }
            }
        }
    }
}

static void mock_task(float f[restrict NB][NZ2][NY2][NX2], FP32_16x8_12x24 *filter) {
    FP32_8x8_24x24 c;
    FP32_8x16_24x12 a;

    memset(&c, 0, sizeof(c));

    for (int y1 = 0; y1 < 24; y1++) {
        for (int x1 = 0; x1 < 12; x1++) {
            for (int y0 = 0; y0 < 8; y0++) {
                for (int x0 = 0; x0 < 16; x0++) {
                    a.elem[y1][x1].rows[y0].cols[x0] = f[SAMPLE_LAYER_N][SAMPLE_LAYER_Z][y1 * 8 + y0][x1 * 16 + x0];
                }
            }
        }
    }

    dp_192x192(&c, &a, filter);

    fprint_8x8_24x24(&c);
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

    init_amx_8x16();

    FP32_16x8_12x24 filter;
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

    release_amx();
    return 0;
}
