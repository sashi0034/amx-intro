
#include "t11_mhd.h"
#include "t11_sim.h"

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

static void mock_task(float f[restrict NB][NZ2][NY2][NX2], Filter3x3 *filter) {
    SimMat output;
    memset(&output, 0, sizeof(output));

    SimMat *input = (SimMat *) (f[SAMPLE_LAYER_N][SAMPLE_LAYER_Z]);

    convolution_naive(&output, input, filter);

    fprint_SimMat_to_file("output/out_naive.txt", &output);
}

// -----------------------------------------------

int main() {
    SimState simState;
    init_sim_state(&simState);

    Filter3x3 filter;
    make_filter(&filter);

    // Main loop
    for (int ii = 1; ii <= LAST; ii++) {
        tick_sim_state(&simState, ii);

        // -----------------------------------------------

        if (ii == 2) {
            printf("----------------------------------------------- %d\n", ii);
            // print_sample_layer(f);
            mock_task(simState.f, &filter);
            break;
        }
    }

    return 0;
}
