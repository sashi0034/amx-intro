#include <immintrin.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdbool.h>
#include <memory.h>

#define MAX_SRC_256 256
#define MAX_DST_64 64
#define MAT_A_ROWS_8 8
#define MAT_A_COLS_32 32

#define MAT_B_ROWS_32 32
#define MAT_B_COLS_8 8

#define STRIDE_32 32
#define DWORD_SIZE_4 4

typedef struct {
    union {
        struct {
            int8_t cols[32];
        } rows[8];
        int8_t bytes[8 * 32];
    };
} bytes8x32_t;

typedef bytes8x32_t bytes32x8transposed_t; // Transposed

typedef struct {
    union {
        struct {
            int8_t cols[8];
        } rows[32];
        int8_t bytes[32 * 8];
    };
} bytes32x8_t;

typedef struct {
    union {
        struct {
            int32_t cols[8];
        } rows[8];
        int32_t dwords[8 * 8];
    };
} dword8x8_t;

typedef struct {
    bytes8x32_t a;
    bytes32x8transposed_t b;
    dword8x8_t c;
} matrix_set_t;

// -----------------------------------------------

void dot_product(dword8x8_t *c, const bytes8x32_t *a, const bytes8x32_t *b) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 32; k++) {
                c->rows[i].cols[j] += a->rows[i].cols[k] * b->rows[j].cols[k];
            }
        }
    }
}

// -----------------------------------------------

typedef struct {
    int64_t cases;
    int8_t *input_buffer;
    int32_t *result_buffer;
} test_buffer_t;

static bool read_test_buffer(test_buffer_t *test_buffer) {
    FILE *file = fopen("test.bin", "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return false;
    }

    fread(&test_buffer->cases, sizeof(test_buffer->cases), 1, file);
    printf("Test cases: %ld\n", test_buffer->cases);

    test_buffer->input_buffer = (int8_t *) malloc(test_buffer->cases * (MAX_SRC_256 * 2) * sizeof(int8_t));
    fread(test_buffer->input_buffer, sizeof(int8_t), test_buffer->cases * (MAX_SRC_256 * 2), file);

    test_buffer->result_buffer = (int32_t *) malloc(test_buffer->cases * (MAX_DST_64) * sizeof(int32_t));
    fread(test_buffer->result_buffer, sizeof(int32_t), test_buffer->cases * (MAX_DST_64), file);

    fclose(file);
    return true;
}

static void init_from_test_buffer(matrix_set_t *matrix_set, test_buffer_t *buffer) {
    for (int t = 0; t < buffer->cases; ++t) {
        matrix_set_t *mat = &matrix_set[t];
        bytes8x32_t *a = &mat->a;
        bytes32x8transposed_t *b = &mat->b;
        for (int i = 0; i < MAX_SRC_256; ++i) {
            a->bytes[i] = *(buffer->input_buffer++);
        }

        for (int c = 0; c < 32; ++c) {
            for (int r = 0; r < 8; r++) {
                b->rows[r].cols[c] = *(buffer->input_buffer++);
            }
        }

        memset(&mat->c, 0, sizeof(dword8x8_t));
    }
}

void compute_dot_products(matrix_set_t *matrix_set, int64_t cases) {
    for (int t = 0; t < cases; ++t) {
        matrix_set_t *mat = &matrix_set[t];
        dot_product(&mat->c, &mat->a, &mat->b);
    }
}

static void check_result_validation(const matrix_set_t *matrix_set, test_buffer_t *buffer) {
    int errors = 0;
    for (int t = 0; t < (*buffer).cases; ++t) {
        const matrix_set_t *mat = &matrix_set[t];

        for (int i = 0; i < MAX_DST_64; ++i) {
            int32_t r = *((*buffer).result_buffer++);
            if (mat->c.dwords[i] != r) errors++;
        }
    }

    printf("Errors: %d\n", errors);
}

int main() {
    test_buffer_t buffer;
    if (!read_test_buffer(&buffer)) {
        printf("Failed to read test buffer\n");
        return -1;
    }

    // Allocate memory for test cases
    matrix_set_t *matrix_set = (matrix_set_t *) malloc(buffer.cases * sizeof(matrix_set_t));

    // Load test cases for matrix product
    init_from_test_buffer(matrix_set, &buffer);

    // Compute dot product for each test case
    compute_dot_products(matrix_set, buffer.cases);

    // Check if the result is correct
    check_result_validation(matrix_set, &buffer);

    free(matrix_set);

    return 0;
}
