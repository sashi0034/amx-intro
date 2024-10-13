#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>
#include <iomanip>

constexpr int TEST_CASES = 50;

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
            std::cout << "0x"
                      << std::setw(2) << std::setfill('0') << std::hex << (static_cast<uint32_t>(elem) & 0xFF)
                      << ", ";
        }
        // std::cout << std::endl;
    }
}

void printMatrixC(const MatrixC &matrix) {
    for (const auto &row: matrix) {
        for (const auto &elem: row) {
            std::cout << std::dec << elem << ", ";
        }
        // std::cout << std::endl;
    }
}

int main() {
    // Random seed
    std::srand(std::time(0));

    std::cout << "#include \"test_cases.h\"\n\n";

    std::cout << "int test_count = " << TEST_CASES << ";\n\n";

    std::cout << "int8_t test_input_bytes[] = {\n\t";

    std::vector<MatrixC> results;
    results.resize(TEST_CASES);

    for (int t = 0; t < TEST_CASES; ++t) {
        MatrixA matrixA;
        MatrixB matrixB;
        generateMatrixA(matrixA);
        generateMatrixB(matrixB);

        printMatrix(matrixA);
        // std::cout << std::endl;

        printMatrix(matrixB);
        // std::cout << std::endl;

        results[t] = multiply(matrixA, matrixB);
    }

    std::cout << "\n};\n" << std::endl;

    std::cout << "int32_t test_result_dwords[] = {\n\t";

    for (int t = 0; t < TEST_CASES; ++t) {
        printMatrixC(results[t]);
    }

    std::cout << "\n};\n\n";

    std::cout << std::endl;

    return 0;
}
