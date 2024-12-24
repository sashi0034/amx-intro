#include <immintrin.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_SRC_256 256
#define MAX_DST_64 64
#define MAT_A_ROWS_8 8
#define MAT_A_COLS_32 32

#define MAT_B_ROWS_32 32
#define MAT_B_COLS_8 8

#define STRIDE_32 32
#define DWORD_SIZE_4 4

typedef struct {
    int8_t cols[32];
} Bytes32;

typedef struct {
    union {
        Bytes32 rows[8];
        int8_t bytes[8 * 32];
    };
} Bytes8x32;

typedef struct {
    int8_t cols[8];
} Bytes8;

typedef struct {
    union {
        Bytes8 rows[32];
        int8_t bytes[32 * 8];
    };
} Bytes32x8;

typedef struct {
    int32_t cols[8];
} Dword8;

typedef struct {
    union {
        Dword8 rows[8];
        int32_t dwords[8 * 8];
    };
} Dword8x8;

typedef struct {
    Bytes8x32 a;
    Bytes32x8 b;
    Dword8x8 c;
} MatrixTuple;

static void init_dword8x8(Dword8x8 *buf, int32_t value) {
    for (int i = 0; i < MAT_A_ROWS_8; i++) {
        for (int j = 0; j < MAT_B_COLS_8; j++) {
            buf->rows[i].cols[j] = value;
        }
    }
}

static void compute_dot_product(Dword8x8 *c, Bytes8x32 *a, Bytes32x8 *b) {
    for (int i = 0; i < MAT_A_ROWS_8; i++) {
        for (int j = 0; j < MAT_B_COLS_8; j++) {
            for (int k = 0; k < MAT_A_COLS_32; k++) {
                c->rows[i].cols[j] += a->rows[i].cols[k] * b->rows[k].cols[j];
            }
        }
    }
}

typedef struct {
    int64_t cases;
    int8_t *input_buffer;
    int32_t *result_buffer;
} TestBuffer;

static bool read_test_buffer(TestBuffer *test_buffer) {
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

static void init_all_tests_from_buffer(MatrixTuple *test_array, TestBuffer *buffer) {
    for (int t = 0; t < buffer->cases; ++t) {
        MatrixTuple *mat = &test_array[t];
        Bytes8x32 *a = &mat->a;
        Bytes32x8 *b = &mat->b;
        for (int i = 0; i < MAX_SRC_256; ++i) {
            a->bytes[i] = *(buffer->input_buffer++);
        }

        for (int i = 0; i < MAX_SRC_256; ++i) {
            b->bytes[i] = *(buffer->input_buffer++);
        }

        init_dword8x8(&mat->c, 0);
    }
}

void compute_all_tests(MatrixTuple *test_array, int cases) {
    for (int t = 0; t < cases; ++t) {
        MatrixTuple *mat = &test_array[t];
        compute_dot_product(&mat->c, &mat->a, &mat->b);
    }
}

static void check_result_validation(const MatrixTuple *test_array, TestBuffer *buffer) {
    int errors = 0;
    for (int t = 0; t < (*buffer).cases; ++t) {
        const MatrixTuple *mat = &test_array[t];

        for (int i = 0; i < MAX_DST_64; ++i) {
            int32_t r = *((*buffer).result_buffer++);
            if (mat->c.dwords[i] != r) errors++;
        }
    }

    printf("Errors: %d\n", errors);
}

int main() {
    TestBuffer buffer;
    if (!read_test_buffer(&buffer)) {
        printf("Failed to read test buffer\n");
        return -1;
    }

    // Allocate memory for test cases
    MatrixTuple *test_array = (MatrixTuple *) malloc(buffer.cases * sizeof(MatrixTuple));

    // Load test cases for matrix product
    init_all_tests_from_buffer(test_array, &buffer);

    // Compute dot product for each test case
    compute_all_tests(test_array, buffer.cases);

    // Check if the result is correct
    check_result_validation(test_array, &buffer);

    free(test_array);

    return 0;
}

