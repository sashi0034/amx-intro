#include <iostream>
#include <vector>
#include <array>
#include "AmxCalculator.h"
#include "Matrix.h"

int main() {
    using namespace amx;

    amx::Bytes16x64 a{};
    amx::Bytes16x64 b{};

    for (int i = 0; i < a.bytes.size(); ++i) {
        a.bytes[i] = 3;
    }

    for (int i = 0; i < b.bytes.size(); ++i) {
        b.bytes[i] = 2;
    }

    amx::AmxCalculator calculator{};
    auto c = calculator.DotProduct(a, b);

    std::cout << a.ToString()
              << "--------\n"
              << b.ToString()
              << "--------\n"
              << c.ToString();
}
