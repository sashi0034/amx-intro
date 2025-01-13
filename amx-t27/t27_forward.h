
#include <immintrin.h>
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define TRACE() printf("[%s:%d] ", __FILE__, __LINE__); fflush(stdout);

#define DEFINE_BYTE_MATRIX(name, r, c) \
    typedef struct name { \
        union { \
            int8_t bytes[(r) * (c)]; \
            struct { \
                int8_t cols[(c)]; \
            } rows[(r)]; \
            int8_t mat[r][c]; \
        }; \
    } name; \
    \
    static void print_##name(const name* mat) { \
        for (int i = 0; i < (r); ++i) { \
            for (int j = 0; j < (c); ++j) { \
                printf("%d ", mat->rows[i].cols[j]); \
            } \
            printf("\n"); \
        } \
    } \
    \
    static void fprint_##name(const name* mat, FILE* stream) { \
        for (int i = 0; i < (r); ++i) { \
            for (int j = 0; j < (c); ++j) { \
                fprintf(stream, "%d ", mat->rows[i].cols[j]); \
            } \
            fprintf(stream, "\n"); \
        } \
    } \
    \
    static void fprint_##name##_to_file(const char* filename, const name* mat) { \
        FILE* file = fopen(filename, "w"); \
        if (file) { \
            fprint_##name(mat, file); \
            fclose(file); \
        } else { \
            perror("Failed to open file"); \
        } \
    }

#define DEFINE_BYTE_MATRIX_N(name, r, c, n) \
    typedef struct name { \
        union { \
            int8_t bytes[(r) * (c) * (n)]; \
            struct { \
                struct { \
                    int8_t ch[(n)]; \
                } cols[(c)]; \
            } rows[(r)]; \
            int8_t mat[r][c][n]; \
        }; \
    } name;

#define DEFINE_DWORD_MATRIX_N(name, r, c, n) \
    typedef struct name { \
        union { \
            int32_t dwords[(r) * (c) * (n)]; \
            struct { \
                struct { \
                    int32_t ch[(n)]; \
                } cols[(c)]; \
            } rows[(r)]; \
            int32_t mat[r][c][n]; \
        }; \
    } name; \
    \
    static void print_##name(const name* mat) { \
        for (int i = 0; i < (r); ++i) { \
            for (int j = 0; j < (c); ++j) { \
                printf("["); \
                for (int k = 0; k < (n); ++k) { \
                    printf("%d", mat->rows[i].cols[j].ch[k]); \
                    if (k < (n) - 1) printf(", "); \
                } \
                printf("] "); \
            } \
            printf("\n"); \
        } \
    } \
    \
    static void fprint_##name(FILE* file, const name* mat) { \
        for (int i = 0; i < (r); ++i) { \
            for (int j = 0; j < (c); ++j) { \
                fprintf(file, "["); \
                for (int k = 0; k < (n); ++k) { \
                    fprintf(file, "%d", mat->rows[i].cols[j].ch[k]); \
                    if (k < (n) - 1) fprintf(file, ", "); \
                } \
                fprintf(file, "] "); \
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

#define INPUT_CH 9

DEFINE_BYTE_MATRIX_N(input_mat_t, INPUT_ROWS, INPUT_COLS, INPUT_CH)

static void init_input_mat(input_mat_t *mat) {
    for (int i = 0; i < INPUT_ROWS; ++i) {
        for (int j = 0; j < INPUT_COLS; ++j) {
            for (int n = 0; n < INPUT_CH; ++n) {
                mat->rows[i].cols[j].ch[n] = (int8_t) (i + j + n);
            }
        }
    }
}

#define FILTER_SIZE 7
#define FILTER_PADDING ((FILTER_SIZE - 1) / 2)

#define FILTER_CH 16 // equals to the number of output channels

DEFINE_BYTE_MATRIX_N(filter7x7_t, FILTER_SIZE, FILTER_SIZE, FILTER_CH)

_Static_assert(sizeof(filter7x7_t) == FILTER_SIZE * FILTER_SIZE * FILTER_CH, "Invalid filter size");

static void init_filter(filter7x7_t filter[INPUT_CH]) {
    for (int f = 0; f < INPUT_CH; ++f) {
        for (int i = 0; i < FILTER_SIZE; ++i) {
            for (int j = 0; j < FILTER_SIZE; ++j) {
                for (int n = 0; n < FILTER_CH; ++n) {
                    filter[f].rows[i].cols[j].ch[n] = (int8_t) (f + i * 2 - j * 3 - n);
                }
            }
        }
    }
}

#define OUTPUT_ROWS INPUT_ROWS // (INPUT_ROWS - FILTER_SIZE + 1)
#define OUTPUT_COLS INPUT_COLS // (INPUT_COLS - FILTER_SIZE + 1)

DEFINE_DWORD_MATRIX_N(output_mat_t, OUTPUT_ROWS, OUTPUT_COLS, FILTER_CH)

_Static_assert(sizeof(output_mat_t) == OUTPUT_ROWS * OUTPUT_COLS * FILTER_CH * sizeof(int32_t), "Invalid output size");

#define CONVOLUTION_COUNT 1000
