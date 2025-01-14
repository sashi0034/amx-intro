
#include "t27_forward.h"

// -----------------------------------------------

void convolution(output_mat_t *output, const input_mat_t *input, const filter7x7_t filter[INPUT_CH]) {
#pragma acc data copyin(input[0:1], filter[0:INPUT_CH]) copyout(output[0:1])
    {
#pragma acc parallel loop collapse(3)
        for (int r = 0; r < INPUT_ROWS - FILTER_PADDING * 2; ++r) {
            for (int c = 0; c < INPUT_COLS - FILTER_PADDING * 2; ++c) {
                for (int fch = 0; fch < FILTER_CH; ++fch) {

                    int32_t sum = 0;
#pragma acc loop collapse(3) reduction(+:sum)
                    for (int fr = 0; fr < FILTER_SIZE; ++fr) {
                        for (int fc = 0; fc < FILTER_SIZE; ++fc) {
                            for (int ich = 0; ich < INPUT_CH; ++ich) {
                                sum += input->rows[r + fr].cols[c + fc].ch[ich] * filter[ich].rows[fr].cols[fc].ch[fch];
                            }
                        }
                    }

                    output->rows[r].cols[c].ch[fch] = sum;
                }
            }
        }
    }
}

// -----------------------------------------------

int main() {
    input_mat_t input;
    init_input_mat(&input);

    output_mat_t output;

    filter7x7_t f[INPUT_CH];
    init_filter(f);

    // -----------------------------------------------

    for (int i = 0; i < CONVOLUTION_COUNT; i++) {
        memset(&output, 0, sizeof(output_mat_t));

        convolution(&output, &input, f);

        for (int j = 0; j < FILTER_CH; ++j) input.bytes[j % INPUT_CH] += (int8_t) (output.rows[0].cols[0].ch[j]);
    }

    // -----------------------------------------------

    fprint_output_mat_t_to_file("out/t26_naive_output.txt", &output);
}
