
#include <stdio.h>
#include <memory.h>
#include "t13_matrix.h"

// -----------------------------------------------

void convolve(DstMat *dst, const SrcMat *src, const Filter3x3 *filter) {
    for (int r = 0; r < SRC_MAT_ROWS - FILTER_PADDING * 2; ++r) {
        for (int c = 0; c < SRC_MAT_COLS - FILTER_PADDING * 2; ++c) {
            float sum = 0;
            for (int i = 0; i < FILTER_SIZE; ++i) {
                for (int j = 0; j < FILTER_SIZE; ++j) {
                    const int r2 = r + i;
                    const int c2 = c + j;
                    sum += src->rows[r2].cols[c2] * filter->rows[i].cols[j];
                }
            }

            dst->rows[r].cols[c] = sum;
        }
    }
}

// -----------------------------------------------

int main() {
    FILE *file = fopen("test.bin", "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    int count;
    fread(&count, sizeof(int), 1, file);

    printf("count: %d\n", count);

    Filter3x3 filter;
    fread(&filter, sizeof(Filter3x3), 1, file);

    SrcMat *mat = malloc(sizeof(SrcMat) * count);
    fread(mat, sizeof(SrcMat), count, file);

    DstMat *dst = malloc(sizeof(DstMat) * count);
    memset(dst, 0, sizeof(DstMat) * count);

    for (int i = 0; i < count; i++) {
        convolve(&dst[i], &mat[i], &filter);
    }

    fclose(file);

    return output_dst(dst, "data/naive.txt");
}
