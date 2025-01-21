
#include <malloc.h>
#include <time.h>
#include "t30_sim.h"

void make_filter(Filter3x3 *filter) {
    for (int r = 0; r < FILTER_SIZE; ++r) {
        for (int c = 0; c < FILTER_SIZE; ++c) {
            filter->rows[r].cols[c] = (float) ((r + c) % 2);
        }
    }
}

void output_conv(const ConvOutput *output, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }

    for (int i = 0; i < LAST; ++i) {
        fprint_SimMat(file, &output->mats[i]);
        fprintf(file, "\n");
    }

    fclose(file);

}

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

void init_sim_state(SimState *state) {
    state->f = (float (*)[NZ2][NY2][NX2]) malloc(NB * NZ2 * NY2 * NX2 * sizeof(float));
    state->u = (float (*)[NZ2][NY2][NX2]) malloc(NB * NZ2 * NY2 * NX2 * sizeof(float));
    state->pp = (float (*)[NZ2][NY2][NX2]) malloc(NBBB * NZ2 * NY2 * NX2 * sizeof(float));

    state->nmlf = 0;
    state->nupb = 0;
    state->nout = 0;
    state->ntap = 10;

    state->vmax = 0.0f;

    // Simulation information
    simuinfo();

    // Record start time
    state->zt0 = clock();

    // Initial conditions
    equil_out(state->f, state->pp);

    // Record initial time
    state->zt1 = clock();
}

void tick_sim_state(SimState *state, int ii) {
    state->nmlf++;
    state->nupb++;
    state->nout++;

    // Outer boundary for f
    bdry_z_m(state->f, state->u);

    // Main calculation
    mlf(state->f, state->u, state->pp, ii, state->nmlf, state->vmax);

    if (state->vmax > 1.0)
        state->nout = NOUT0;

    if (state->nmlf == NMLF0)
        state->nmlf = 0;

    // Upward boundary condition for f
    if (state->nupb == NUPB0) {
        state->nupb = 0;
        bdry_up(state->f, state->u);
    }

    // Finalization part (write data, etc.)
    if (state->nout == NOUT0) {
        state->nout = 0;

        // Record end time
        state->zt3 = clock();

        // Output information
        state->ntap++;

        printf("    ii, LAST, ntap,   vmax,    THX\n");
        printf("%6d%6d%6d%8.4f%8.4f\n", ii, LAST, state->ntap, state->vmax, THX);
        printf(" \n");

        state->zt = (double) (state->zt3 - state->zt1) / CLOCKS_PER_SEC;
        state->ztt = (double) (state->zt1 - state->zt0) / CLOCKS_PER_SEC;

        printf("    ii,      init,        zt\n");
        printf("%6d%11.4f%11.4f\n", ii, state->ztt, state->zt);
        printf(" \n");

        gflops(state->zt);
    }
}
