#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>
#include <fstream>

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

int main(int argc, char *argv[]) {
    // Random seed
    std::srand(std::time(0));

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <test_cases>" << std::endl;
        return 1;
    }

    const uint64_t test_cases = std::atoi(argv[1]);

    std::vector<MatrixC> results;
    results.resize(test_cases);

    std::ofstream ofs("test.bin", std::ios::binary);
    if (!ofs.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return 1;
    }

    ofs.write(reinterpret_cast<const char *>(&test_cases), sizeof(test_cases));

    for (int t = 0; t < test_cases; ++t) {
        MatrixA matrixA;
        MatrixB matrixB;
        generateMatrixA(matrixA);
        generateMatrixB(matrixB);

        ofs.write(reinterpret_cast<const char *>(matrixA.data()), sizeof(matrixA));
        ofs.write(reinterpret_cast<const char *>(matrixB.data()), sizeof(matrixB));

        results[t] = multiply(matrixA, matrixB);
    }

    for (int t = 0; t < test_cases; ++t) {
        ofs.write(reinterpret_cast<const char *>(results[t].data()), sizeof(results[t]));
    }

    ofs.close();
    return 0;
}
