
#include "t11_mhd.h"
#include "t11_sim.h"

#include <stdio.h>
#include <memory.h>
#include <openacc.h>

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

static void mock_task(const float f[restrict NB][NZ2][NY2][NX2], const Filter3x3 *filter) {
    SimMat output;
    memset(&output, 0, sizeof(output));

    convolution_gpu(output.mat, f[SAMPLE_LAYER_N][SAMPLE_LAYER_Z], filter->mat);

    fprint_SimMat_to_file("output/out_gpu.txt", &output);
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

        if (ii == 2) {
            printf("----------------------------------------------- %d\n", ii);
            // print_sample_layer(f);
            mock_task(simState.f, &filter);
            break;
        }
    }

    return 0;
}