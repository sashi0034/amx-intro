
#include <immintrin.h>
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

#define DEFINE_MATRIX(name, r, c) \
    typedef struct name { \
        union { \
            float fp32s[(r) * (c)]; \
            struct { \
                float cols[(c)]; \
            } rows[(r)];          \
            float mat[r][c]; \
        }; \
    } name; \
    \
    static void print_##name(const name* mat) { \
        for (int i = 0; i < (r); ++i) { \
            for (int j = 0; j < (c); ++j) { \
                printf("%f ", (mat->rows[i].cols[j])); \
            } \
            printf("\n"); \
        } \
    } \
    \
    static void fprint_##name(FILE* file, const name* mat) { \
        for (int i = 0; i < (r); ++i) { \
            for (int j = 0; j < (c); ++j) { \
                fprintf(file, "%f ", (mat->rows[i].cols[j])); \
            } \
            fprintf(file, "\n"); \
        } \
    } \
    \
    static void fprint_##name##_to_file(const char* filename, const name* mat) { \
        FILE* file = fopen(filename, "w"); \
        if (!file) { \
            perror("Failed to open file"); \
            return; \
        } \
        fprint_##name(file, mat); \
        fclose(file); \
    }

#define MATRIX_ROWS 192
#define MATRIX_COLS 192

DEFINE_MATRIX(matrix_t, MATRIX_ROWS, MATRIX_COLS)

#define FILTER_SIZE 3
#define FILTER_OFFSET ((FILTER_SIZE - 1) / 2)

DEFINE_MATRIX(filter3x3_t, FILTER_SIZE, FILTER_SIZE)

#define CONVOLUTION_COUNT 100000
