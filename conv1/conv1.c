
#include <stdio.h>

#define DEFINE_MATRIX(NAME, WIDTH, HEIGHT) \
    typedef struct NAME { \
        union { \
            float fp32s[(WIDTH) * (HEIGHT)]; \
            struct { \
                float cols[(WIDTH)]; \
            } rows[(HEIGHT)]; \
        }; \
    } NAME;

#define MATRIX_ROWS 7
#define MATRIX_COLS 7

DEFINE_MATRIX(Matrix, MATRIX_ROWS, MATRIX_COLS)

#define FILTER_SIZE 3
#define FILTER_OFFSET ((FILTER_SIZE - 1) / 2)

DEFINE_MATRIX(Filter3x3, FILTER_SIZE, FILTER_SIZE)

void print_matrix(const Matrix *m) {
    for (int r = 0; r < MATRIX_ROWS; ++r) {
        for (int c = 0; c < MATRIX_COLS; ++c) {
            printf("%f ", m->rows[r].cols[c]);
        }
        printf("\n");
    }
}

// -----------------------------------------------

void convolution_naive(Matrix *output, const Matrix *input, const Filter3x3 *filter) {
    for (int r = 0; r < MATRIX_ROWS - FILTER_OFFSET * 2; ++r) {
        for (int c = 0; c < MATRIX_COLS - FILTER_OFFSET * 2; ++c) {
            float sum = 0;
            for (int fr = 0; fr < FILTER_SIZE; ++fr) {
                for (int fc = 0; fc < FILTER_SIZE; ++fc) {
                    sum += input->rows[r + fr].cols[c + fc] * filter->rows[fr].cols[fc];
                }
            }

            output->rows[FILTER_OFFSET + r].cols[FILTER_OFFSET + c] = sum;
        }
    }
}

// -----------------------------------------------

int main() {
    Matrix input;
    for (int r = 0; r < MATRIX_ROWS; ++r) {
        for (int c = 0; c < MATRIX_COLS; ++c) {
            input.rows[r].cols[c] = (float) (r * MATRIX_COLS + c);
        }
    }

    Matrix output;
    for (int r = 0; r < MATRIX_ROWS; ++r) {
        for (int c = 0; c < MATRIX_COLS; ++c) {
            output.rows[r].cols[c] = 0.0f;
        }
    }

    Filter3x3 f;
    for (int r = 0; r < FILTER_SIZE; ++r) {
        for (int c = 0; c < FILTER_SIZE; ++c) {
            f.rows[r].cols[c] = (float) ((r + c) % 2);
        }
    }

    convolution_naive(&output, &input, &f);

    print_matrix(&output);
}

