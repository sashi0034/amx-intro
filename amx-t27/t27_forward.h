
#include <immintrin.h>
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

#define DEFINE_BYTE_MATRIX(name, r, c) \
    typedef struct name { \
        union { \
            int8_t bytes[(r) * (c)]; \
            struct { \
                int8_t cols[(c)]; \
            } rows[(r)];          \
            int8_t* mat[r][c]; \
        }; \
    } name;

#define DEFINE_DWORD_MATRIX(name, r, c) \
    typedef struct name { \
        union { \
            int32_t dwords[(r) * (c)]; \
            struct { \
                int32_t cols[(c)]; \
            } rows[(r)];          \
            int32_t mat[r][c]; \
        }; \
    } name; \
    \
    static void print_##name(const name* mat) { \
        for (int i = 0; i < (r); ++i) { \
            for (int j = 0; j < (c); ++j) { \
                printf("%d ", (mat->rows[i].cols[j])); \
            } \
            printf("\n"); \
        } \
    } \
    \
    static void fprint_##name(FILE* file, const name* mat) { \
        for (int i = 0; i < (r); ++i) { \
            for (int j = 0; j < (c); ++j) { \
                fprintf(file, "%d ", (mat->rows[i].cols[j])); \
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

#define INPUT_ROWS 192
#define INPUT_COLS 192

DEFINE_BYTE_MATRIX(input_mat_t, INPUT_ROWS, INPUT_COLS)

#define FILTER_SIZE 7
#define FILTER_OFFSET ((FILTER_SIZE - 1) / 2)

DEFINE_BYTE_MATRIX(filter7x7_t, FILTER_SIZE, FILTER_SIZE)

#define OUTPUT_ROWS INPUT_ROWS // (INPUT_ROWS - FILTER_SIZE + 1)
#define OUTPUT_COLS INPUT_COLS // (INPUT_COLS - FILTER_SIZE + 1)
DEFINE_DWORD_MATRIX(output_mat_t, OUTPUT_ROWS, OUTPUT_COLS)

#define CONVOLUTION_COUNT 1
