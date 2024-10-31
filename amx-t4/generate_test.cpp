#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>

constexpr int test_cases = 500000;

constexpr int ROWS_A_8 = 8;
constexpr int COLS_A_32 = 32;
constexpr int ROWS_B_32 = COLS_A_32;
constexpr int COLS_B_8 = 8;

using MatrixA = std::array<std::array<int8_t, COLS_A_32>, ROWS_A_8>;
using MatrixB = std::array<std::array<int8_t, COLS_B_8>, ROWS_B_32>;
using MatrixC = std::array<std::array<int32_t, COLS_B_8>, ROWS_A_8>;

void generateMatrixA(MatrixA &matrix) {
    for (auto &row: matrix) {
        for (auto &elem: row) {
            elem = rand() % 256 - 128; // [-128, 127]
        }
    }
}

void generateMatrixB(MatrixB &matrix) {
    for (auto &row: matrix) {
        for (auto &elem: row) {
            elem = rand() % 256 - 128; // [-128, 127]
        }
    }
}

MatrixC multiply(const MatrixA &a, const MatrixB &b) {
    MatrixC result = {};
    for (int i = 0; i < ROWS_A_8; ++i) {
        for (int j = 0; j < COLS_B_8; ++j) {
            int sum = 0;
            for (int k = 0; k < COLS_A_32; ++k) {
                sum += a[i][k] * b[k][j];
            }
            result[i][j] = sum;
        }
    }

    return result;
}

template<typename MatrixType>
void printMatrix(const MatrixType &matrix) {
    for (const auto &row: matrix) {
        for (const auto &elem: row) {
            std::cout << static_cast<int>(elem) << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    // Random seed
    std::srand(std::time(0));

    std::cout << test_cases << std::endl << std::endl;

    std::vector<MatrixC> results;
    results.resize(test_cases);

    for (int t = 0; t < test_cases; ++t) {
        MatrixA matrixA;
        MatrixB matrixB;
        generateMatrixA(matrixA);
        generateMatrixB(matrixB);

        printMatrix(matrixA);
        std::cout << std::endl;

        printMatrix(matrixB);
        std::cout << std::endl;

        results[t] = multiply(matrixA, matrixB);
    }

    for (int t = 0; t < test_cases; ++t) {
        printMatrix(results[t]);
        std::cout << std::endl;
    }

    return 0;
}
