
#include "t30_mhd.h"
#include "t30_sim.h"

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <openacc.h>

static void convolution_naive(SimMat *output, const SimMat *input, const Filter3x3 *filter) {
    for (int r = 0; r < SIM_MAT_ROWS - FILTER_PADDING * 2; r++) {
        for (int c = 0; c < SIM_MAT_COLS - FILTER_PADDING * 2; c++) {
            float sum = 0.0f;
            for (int fi = 0; fi < FILTER_SIZE; fi++) {
                for (int fj = 0; fj < FILTER_SIZE; fj++) {
                    sum += input->rows[r + fi].cols[c + fj] * filter->rows[fi].cols[fj];
                }
            }

            output->rows[r].cols[c] = sum;
        }
    }
}

static void mock_task(SimMat *output, float f[restrict NB][NZ2][NY2][NX2], const Filter3x3 *filter) {
    SimMat *input = (SimMat *) (f[SAMPLE_LAYER_N][SAMPLE_LAYER_Z]);

    convolution_naive(output, input, filter);
}

// -----------------------------------------------

int main() {
    SimState simState;
    init_sim_state(&simState);

    Filter3x3 filter;
    make_filter(&filter);

    ConvOutput output;
    memset(&output, 0, sizeof(ConvOutput));

    // Main loop
    for (int ii = 1; ii <= LAST; ii++) {
        tick_sim_state(&simState, ii);

        mock_task(&output.mats[ii - 1], simState.f, &filter);
    }

    output_conv(&output, "output/out_naive.txt");

    return 0;
}
