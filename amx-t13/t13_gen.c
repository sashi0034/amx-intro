#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

#define FILTER_SIZE 3
#define FILTER_PADDING ((FILTER_SIZE - 1) / 2)

DEFINE_MATRIX(Filter3x3, FILTER_SIZE, FILTER_SIZE)

#define SIM_MAT_ROWS 192
#define SIM_MAT_COLS 192
DEFINE_MATRIX(SimMat, SIM_MAT_ROWS, SIM_MAT_COLS)

static void generate_random_Filter3x3(Filter3x3 *filter) {
    for (int i = 0; i < FILTER_SIZE; ++i) {
        for (int j = 0; j < FILTER_SIZE; ++j) {
            filter->mat[i][j] = (float) rand() / RAND_MAX;
        }
    }
}

static void generate_random_SimMat(SimMat *mat) {
    for (int i = 0; i < SIM_MAT_ROWS; ++i) {
        for (int j = 0; j < SIM_MAT_COLS; ++j) {
            mat->mat[i][j] = (float) rand() / RAND_MAX;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <count>\n", argv[0]);
        return 1;
    }

    int count = atoi(argv[1]);
    if (count <= 0) {
        fprintf(stderr, "Invalid count value. Must be a positive integer.\n");
        return 1;
    }

    srand((unsigned) time(NULL));

    FILE *file = fopen("test.bin", "wb");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    // Write count to file
    fwrite(&count, sizeof(int), 1, file);

    // Generate and write a random Filter3x3 to file
    Filter3x3 filter;
    generate_random_Filter3x3(&filter);
    fwrite(filter.fp32s, sizeof(float), FILTER_SIZE * FILTER_SIZE, file);

    // Generate and write count SimMats to file
    for (int i = 0; i < count; ++i) {
        SimMat mat;
        generate_random_SimMat(&mat);
        fwrite(mat.fp32s, sizeof(float), SIM_MAT_ROWS * SIM_MAT_COLS, file);
    }

    fclose(file);
    printf("Data written to test.bin successfully.\n");

    return 0;
}
