#include <stdio.h>

#define SIZE 16
#define MATRIX_SIZE (SIZE * SIZE)

// 行列の乗算
void multiplyMatrices(int a[restrict MATRIX_SIZE], int b[restrict MATRIX_SIZE], int result[restrict MATRIX_SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            result[i * SIZE + j] = 0;
            for (int k = 0; k < SIZE; k++) {
                result[i * SIZE + j] += a[i * SIZE + k] * b[k * SIZE + j];
            }
        }
    }
}

// 行列の表示
void printMatrix(int matrix[restrict MATRIX_SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", matrix[i * SIZE + j]);
        }
        printf("\n");
    }
}

int main() {
    int matrix1[MATRIX_SIZE];
    int matrix2[MATRIX_SIZE];
    int result[MATRIX_SIZE];

    printf("Input Matrix 1: (%d x %d):\n", SIZE, SIZE);
    for (int i = 0; i < MATRIX_SIZE; i++) {
        scanf("%d", &matrix1[i]);
    }

    printf("Input Matrix 2: (%d x %d):\n", SIZE, SIZE);
    for (int i = 0; i < MATRIX_SIZE; i++) {
        scanf("%d", &matrix2[i]);
    }

    // 行列の乗算
    multiplyMatrices(matrix1, matrix2, result);
    printf("Multiply:\n");
    printMatrix(result);

    return 0;
}
