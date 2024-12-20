#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "t13_matrix.h"

static void generate_random_Filter3x3(Filter3x3 *filter) {
    for (int i = 0; i < FILTER_SIZE; ++i) {
        for (int j = 0; j < FILTER_SIZE; ++j) {
            filter->mat[i][j] = (float) rand() / RAND_MAX;
        }
    }
}

static void generate_random_SrcMat(SrcMat *mat) {
    for (int i = 0; i < SRC_MAT_ROWS; ++i) {
        for (int j = 0; j < SRC_MAT_COLS; ++j) {
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
    fwrite(filter.fp32s, sizeof(Filter3x3), 1, file);

    // Generate and write count SimMats to file
    for (int i = 0; i < count; ++i) {
        SrcMat mat;
        generate_random_SrcMat(&mat);
        fwrite(mat.fp32s, sizeof(SrcMat), 1, file);
    }

    fclose(file);
    printf("Data written to test.bin successfully.\n");

    return 0;
}
