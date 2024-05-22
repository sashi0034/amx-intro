#ifndef AMX_INTRO_MATRIX_H
#define AMX_INTRO_MATRIX_H

#include <cstdint>
#include <array>
#include <cstring>
#include <immintrin.h>

namespace amx {
    struct Wards16x16;

    struct Bytes16x64 {
        using value_type = int8_t;

        struct Row {
            std::array<value_type, 64> cols;
        };

        union {
            std::array<value_type, 16 * 64> bytes;
            std::array<Row, 16> rows;
        };

        Bytes16x64() = default;

        bool operator==(const Bytes16x64 &other) const {
            return memcmp(bytes.data(), other.bytes.data(), sizeof(bytes)) == 0;
        }

        Wards16x16 operator*(const Bytes16x64 &other) const;

        std::string ToString() const;
    };

    struct Wards16x16 {
        using value_type = int32_t;

        struct Row {
            std::array<value_type, 16> cols;
        };

        union {
            std::array<value_type, 16 * 16> words;
            std::array<Row, 16> rows;
        };

        bool operator==(const Wards16x16 &other) const {
            return memcmp(words.data(), other.words.data(), sizeof(words)) == 0;
        }

        std::string ToString() const;
    };

    struct Bf16_16x32 {
        // __m512bh データ型は 32 個の BF16 値を格納するための 512 ビットのレジスタを表す
        using vector_type = __m512bh;

        union {
            std::array<uint16_t, 16 * 32> shorts;
            std::array<vector_type, 16> rows;
        };
    };
}

#endif //AMX_INTRO_MATRIX_H
