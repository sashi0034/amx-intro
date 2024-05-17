#include <iostream>
#include <array>
#include <functional>
#include <chrono>
#include "AmxCalculator.h"
#include "Matrix.h"
#include "Utils.h"

namespace {
    using namespace amx;
    using namespace util;

    constexpr int testCaseCount = 65536;

    void calculateTask(const std::function<Wards16x16(Bytes16x64, Bytes16x64)> &func) {
        amx::Bytes16x64 a{};
        amx::Bytes16x64 b{};

        int testCases{};
        std::cin >> testCases;
        std::cout << "Test cases: " << testCases << "\n";

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < testCases; ++i) {
            for (int i = 0; i < a.bytes.size(); ++i) {
                std::cin >> a.bytes[i];
            }

            for (int i = 0; i < b.bytes.size(); ++i) {
                std::cin >> b.bytes[i];
            }

            const auto c = func(a, b);

            std::cout << c.ToString() << "\n";
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::cout << "Done in " << duration.count() << " seconds\n";
    }

    // AMX 無しで愚直に行列計算
    void runPlain() {
        calculateTask([](const Bytes16x64 &a, const Bytes16x64 &b) {
            return a * b;
        });
    }

    // AMX を使用して行列計算
    void runAmx() {
        amx::AmxCalculator calculator{};
        calculateTask([&](const Bytes16x64 &a, const Bytes16x64 &b) {
            return calculator.DotProduct(a, b);
        });
    }

    // テスト用の行列を出力
    void generateTests() {
        std::cout << testCaseCount << "\n";

        for (int i = 0; i < testCaseCount * 2; ++i) {
            amx::Bytes16x64 a{};
            for (int j = 0; j < a.bytes.size(); ++j) {
                a.bytes[j] = GetRandomByte();
            }
            std::cout << a.ToString() << "\n";
        }
    }
}

int main(int argc, char **argv) {
    std::string_view mode{"--plain"};

    if (argc == 2) {
        mode = argv[1];
    }

    if (mode == "--plain") {
        std::cout << "Running in plain mode\n";
        runPlain();
    } else if (mode == "--amx") {
        std::cout << "Running in AMX mode\n";
        runAmx();
    } else if (mode == "-g") {
        generateTests();
    }
    return 0;
}
