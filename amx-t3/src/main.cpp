#include <iostream>
#include "Matrix.h"
#include "Utils.h"
#include "AmxCalculator.h"

namespace {
    using namespace amx;
    using namespace util;

    void task3() {
        InitAmxCalculation();

        Bytes16x64 a{};
        Bytes16x64 b{};

//        for (int j = 0; j < a.bytes.size(); ++j) {
//            a.bytes[j] = -2;
//            b.bytes[j] = 2;
//        }
//
//        a.rows[0].cols[15] = 0;
//        a.rows[0].cols[16] = 0;
//        a.rows[2].cols[4] = 0;

//        for (int j = 0; j < a.bytes.size(); ++j) {
//            a.bytes[j] = GetRandomByte();
//            b.bytes[j] = GetRandomByte();
//        }

        for (int j = 0; j < 20; ++j) {
            a.bytes[j] = 1;
            b.bytes[j] = 1;
        }

        const auto c1 = a * b;
        const auto c2 = ComputeAmxDotProduct(a, b);

        std::cout << a.ToString() << "\n*\n" << b.ToString()
                  << "======================== plain\n"
                  << c1.ToString() << "\n"
                  << "======================== amx\n"
                  << c2.ToString() << "\n"
                  << "========================\n"
                  << (c1 == c2 ? "OK" : "FAIL") << "\n";
    }
}

int main(int argc, char **argv) {
    task3();

    return 0;
}
