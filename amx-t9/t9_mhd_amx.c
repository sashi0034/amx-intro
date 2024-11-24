
#include "t9_mhd.h"

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

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
//    for (int y = 0; y < NY2; y++) {
//        for (int x = 0; x < NX2; x++) {
//            if (f[SAMPLE_LAYER_N][SAMPLE_LAYER_Z][y][x] != 0) {
//                printf("%f ", f[SAMPLE_LAYER_N][SAMPLE_LAYER_Z][y][x]);
//            }
//        }
//        printf("\n");
//    }
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
            print_sample_layer(f);
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
