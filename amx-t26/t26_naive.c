
#include "t26_forward.h"

// -----------------------------------------------

void convolution_naive(output_mat_t *output, const input_mat_t *input, const filter7x7_t *filter) {
    for (int r = 0; r < INPUT_ROWS - FILTER_OFFSET * 2; ++r) {
        for (int c = 0; c < INPUT_COLS - FILTER_OFFSET * 2; ++c) {
            int32_t sum = 0;
            for (int fr = 0; fr < FILTER_SIZE; ++fr) {
                for (int fc = 0; fc < FILTER_SIZE; ++fc) {
                    sum += input->rows[r + fr].cols[c + fc] * filter->rows[fr].cols[fc];
                }
            }

            output->rows[r].cols[c] = sum;
        }
    }
}

// -----------------------------------------------

int main() {
    input_mat_t input;
    for (int r = 0; r < INPUT_ROWS; ++r) {
        for (int c = 0; c < INPUT_COLS; ++c) {
            input.rows[r].cols[c] = (int8_t) (((r * INPUT_COLS + c) % 256) - 128);
        }
    }

    output_mat_t output;

    filter7x7_t f;
    for (int r = 0; r < FILTER_SIZE; ++r) {
        for (int c = 0; c < FILTER_SIZE; ++c) {
            f.rows[r].cols[c] = (int8_t) ((r + c) % 2);
        }
    }

    // -----------------------------------------------

    for (int i = 0; i < CONVOLUTION_COUNT; i++) {
        memset(&output, 0, sizeof(output_mat_t));

        convolution_naive(&output, &input, &f);

        f.bytes[0] = (int8_t) (output.rows[0].cols[0]);
    }

    // -----------------------------------------------

    fprint_output_mat_t_to_file("out/t26_naive_output.txt", &output);
}
