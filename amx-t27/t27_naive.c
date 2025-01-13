
#include "t27_forward.h"

// -----------------------------------------------

void convolution(output_mat_t *output, const input_mat_t *input, const filter7x7_t filter[INPUT_CH]) {
    for (int r = 0; r < INPUT_ROWS - FILTER_PADDING * 2; ++r) {
        for (int c = 0; c < INPUT_COLS - FILTER_PADDING * 2; ++c) {
            for (int fn = 0; fn < FILTER_CH; ++fn) {

                int32_t sum = 0;
                for (int fr = 0; fr < FILTER_SIZE; ++fr) {
                    for (int fc = 0; fc < FILTER_SIZE; ++fc) {
                        for (int in = 0; in < INPUT_CH; ++in) {
                            sum += input->rows[r + fr].cols[c + fc].ch[in] * filter[in].rows[fr].cols[fc].ch[fn];
                        }
                    }
                }

                output->rows[r].cols[c].ch[fn] = sum;
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
