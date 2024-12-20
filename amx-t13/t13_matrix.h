#include <stdio.h>
#include <stdlib.h>

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

#define SRC_MAT_ROWS 192
#define SRC_MAT_COLS 192

DEFINE_MATRIX(SrcMat, SRC_MAT_ROWS, SRC_MAT_COLS)

typedef SrcMat DstMat;

static int output_dst(const DstMat *dsts, const char *output_path) {
    FILE *file = fopen("choice.txt", "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    int count;
    if (fscanf(file, "%d", &count) != 1) {
        fprintf(stderr, "Failed to read count\n");
        fclose(file);
        return 1;
    }

    FILE *output_file = fopen(output_path, "w");
    if (output_file == NULL) {
        perror("Failed to open output file");
        fclose(file);
        return 1;
    }

    printf("choices: %d\n", count);

    // Read `count` ints from the file and print them
    for (int i = 0; i < count; i++) {
        int index;
        if (fscanf(file, "%d", &index) != 1) {
            fprintf(stderr, "Failed to read index %d\n", i);
            fclose(file);
            fclose(output_file);
            return 1;
        }

        fprintf(output_file, "[%d] %d\n", i, index);
        fprint_SrcMat(output_file, &dsts[index]);
    }

    fclose(file);
    fclose(output_file);

    printf("Finished writing to %s\n", output_path);

    return 0;
}
