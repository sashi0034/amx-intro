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

int main() {
    int test_cases;
    scanf("%d", &test_cases);
    printf("Test cases: %d\n", test_cases);

    // Allocate memory for test cases
    MatrixTuple *test_array = (MatrixTuple *) malloc(test_cases * sizeof(MatrixTuple));

    // Load test cases for matrix product
    for (int t = 0; t < test_cases; ++t) {
        MatrixTuple *test = &test_array[t];
        Bytes8x32 *a = &test->a;
        Bytes32x8 *b = &test->b;
        for (int i = 0; i < MAX_SRC_256; ++i) {
            scanf("%hhd", &a->bytes[i]);
        }

        for (int i = 0; i < MAX_SRC_256; ++i) {
            scanf("%hhd", &b->bytes[i]);
        }

        init_dword8x8(&test->c, 0);
    }

    for (int t = 0; t < test_cases; ++t) {
        // Load test from memory
        MatrixTuple *test = &test_array[t];
        compute_dot_product(&test->c, &test->a, &test->b);
    }

    int errors = 0;
    for (int t = 0; t < test_cases; ++t) {
        // Load test from memory
        MatrixTuple *test = &test_array[t];

        for (int i = 0; i < MAX_DST_64; ++i) {
            int32_t s;
            scanf("%d", &s);
            if (test->c.dwords[i] != s) errors++;
        }
    }

    printf("Errors: %d\n", errors);

    free(test_array);

    return 0;
}

