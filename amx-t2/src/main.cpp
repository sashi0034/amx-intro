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

    constexpr int testCaseCount = 50000;

    void calculateTask(const std::function<Wards16x16(Bytes16x64, Bytes16x64)> &func) {
        amx::Bytes16x64 a{};
        amx::Bytes16x64 b{};

        int testCases{};
        std::cin >> testCases;
        std::cout << "Test cases: " << testCases << "\n";

        auto start = std::chrono::high_resolution_clock::now();

        std::vector<Wards16x16> results{};
        results.reserve(testCases);

        // テストケースを読み込み、すべての計算結果を保存
        for (int i = 0; i < testCases; ++i) {
            for (int j = 0; j < a.bytes.size(); ++j) {
                std::cin >> a.bytes[j];
            }

            for (int j = 0; j < b.bytes.size(); ++j) {
                std::cin >> b.bytes[j];
            }

            const auto c = func(a, b);
            results.emplace_back(c);
        }

        // 計算時間を表示
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::cout << "Done in \n========================\n\n"
                  << duration.count()
                  << " seconds\n\n========================\n";

        // 必要に応じて計算結果を表示
        char show;
        std::cout << "Show results? (Y/n): ";
        std::cin >> show;
        if (show == 'Y' || show == 'y') {
            for (const auto &result: results) {
                std::cout << result.ToString() << "\n";
            }
        }
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
