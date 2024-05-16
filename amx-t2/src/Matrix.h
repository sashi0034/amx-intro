#ifndef AMX_INTRO_MATRIX_H
#define AMX_INTRO_MATRIX_H

#include <cstdint>
#include <array>
#include <cstring>

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

        std::string ToString() const;
    };
}

#endif //AMX_INTRO_MATRIX_H
