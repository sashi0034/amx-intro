
#include "t9_mhd.h"

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

typedef struct FP32_V192 {
    fp32_t cols[192];
} FP32_V192;

typedef struct FP32_192x192 {
    union {
        FP32_V192 rows[192];
    };
} FP32_192x192;

static void dp_192x192(FP32_192x192 *c, const FP32_192x192 *a, const FP32_192x192 *b) {
#pragma acc data copyin(a[0:1], b[0:1]) copyout(c[0:1])
    {
#pragma acc parallel loop collapse(2)
        for (int i = 0; i < 192; i++) {
            for (int j = 0; j < 192; j++) {
                c->rows[i].cols[j] = 0.0f;
                for (int k = 0; k < 192; k++) {
                    c->rows[i].cols[j] += a->rows[i].cols[k] * b->rows[k].cols[j];
                }
            }
        }
    }
}

static void print_192x192(FP32_192x192 *c) {
    for (int i = 0; i < 192; i++) {
        for (int j = 0; j < 192; j++) {
            printf("%f ", c->rows[i].cols[j]);
        }
        printf("\n");
    }
}

void make_filter(FP32_192x192 *filter) {
    for (int i = 0; i < 192; i++) {
        for (int j = 0; j < 192; j++) {
            filter->rows[i].cols[j] = (fp32_t) ((i + j) % 2);
        }
    }
}

static void mock_task(float f[restrict NB][NZ2][NY2][NX2], FP32_192x192 *filter) {
    FP32_192x192 c;
    FP32_192x192 a;

    memset(&c, 0, sizeof(c));

    for (int i = 0; i < 192; i++) {
        for (int j = 0; j < 192; j++) {
            a.rows[i].cols[j] = (fp32_t) f[SAMPLE_LAYER_N][SAMPLE_LAYER_Z][i][j];
        }
    }

    dp_192x192(&c, &a, filter);

    print_192x192(&c);
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

    FP32_192x192 filter;
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

        if (ii == 8) {
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
