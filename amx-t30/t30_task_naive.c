
#include "t30_mhd.h"
#include "t30_sim.h"

#include <memory.h>

static void convolution_naive(
        float output[SIM_MAT_ROWS][SIM_MAT_COLS],
        const float input[SIM_MAT_ROWS][SIM_MAT_COLS],
        const float filter[FILTER_SIZE][FILTER_SIZE]) {
    {
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

static void mock_task(SimMat *output, const float f[restrict NB][NZ2][NY2][NX2], const Filter3x3 *filter) {
    convolution_naive(output->mat, f[SAMPLE_LAYER_N][SAMPLE_LAYER_Z], filter->mat);
}

// -----------------------------------------------

int main() {
    SimState simState;
    init_sim_state(&simState);

    Filter3x3 filter;
    make_filter(&filter);

    output_t output;
    memset(&output, 0, sizeof(output_t));

    // Main loop
    for (int ii = 1; ii <= LAST; ii++) {
        tick_sim_state(&simState, ii);

        mock_task(&output.mats[ii - 1], simState.f, &filter);
    }

    output_conv(&output, "output/out_naive.txt");

    return 0;
}
