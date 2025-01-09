
#include <immintrin.h>
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

#define DEFINE_MATRIX(name, r, c)                      \
    typedef struct name {                              \
        union {                                        \
            float fp32s[(r) * (c)];                    \
            struct {                                   \
                float cols[(c)];                       \
            } rows[(r)];                               \
        };                                             \
    } name;                                            \
                                                       \
    static void print_##name(const name *mat) {               \
        for (int i = 0; i < (r); ++i) {                \
            for (int j = 0; j < (c); ++j) {            \
                printf("%f ", (mat->rows[i].cols[j])); \
            }                                          \
            printf("\n");                              \
        }                                              \
    }

#define MATRIX_ROWS 192
#define MATRIX_COLS 192

DEFINE_MATRIX(input_matrix_t, MATRIX_ROWS, MATRIX_COLS)

#define FILTER_SIZE 3
#define FILTER_OFFSET ((FILTER_SIZE - 1) / 2)

DEFINE_MATRIX(filter3x3_t, FILTER_SIZE, FILTER_SIZE)

