
#include "t25_forward.h"

// -----------------------------------------------

void convolution_naive(matrix_t *output, const matrix_t *input, const filter3x3_t *filter) {
    for (int r = 0; r < MATRIX_ROWS - FILTER_OFFSET * 2; ++r) {
        for (int c = 0; c < MATRIX_COLS - FILTER_OFFSET * 2; ++c) {
            float sum = 0;
            for (int fr = 0; fr < FILTER_SIZE; ++fr) {
                for (int fc = 0; fc < FILTER_SIZE; ++fc) {
                    sum += input->rows[r + fr].cols[c + fc] * filter->rows[fr].cols[fc];
                }
            }

            output->rows[r].cols[c] = sum;
            // output->rows[FILTER_OFFSET + r].cols[FILTER_OFFSET + c] = sum;
        }
    }
}

// -----------------------------------------------

int main() {
    matrix_t input;
    for (int r = 0; r < MATRIX_ROWS; ++r) {
        for (int c = 0; c < MATRIX_COLS; ++c) {
            input.rows[r].cols[c] = (float) (r * MATRIX_COLS + c);
        }
    }

    matrix_t output;
    for (int r = 0; r < MATRIX_ROWS; ++r) {
        for (int c = 0; c < MATRIX_COLS; ++c) {
            output.rows[r].cols[c] = 0.0f;
        }
    }

    filter3x3_t f;
    for (int r = 0; r < FILTER_SIZE; ++r) {
        for (int c = 0; c < FILTER_SIZE; ++c) {
            f.rows[r].cols[c] = (float) ((r + c) % 2);
        }
    }

    // -----------------------------------------------

    convolution_naive(&output, &input, &f);

    // -----------------------------------------------

    _tile_release();

    // -----------------------------------------------

    // print_matrix_t(&output);
    fprint_matrix_t_to_file("out/t25_naive_output.txt", &output);
}
