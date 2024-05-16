#include <sstream>
#include "Matrix.h"

namespace amx {
    Wards16x16 Bytes16x64::operator*(const Wards16x16 &other) const {
        Wards16x16 result;
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                result.rows[i].cols[j] = 0;
                for (int k = 0; k < 64; k++) {
                    result.rows[i].cols[j] += (*this).rows[i].cols[k] * other.rows[j].cols[k];
                }
            }
        }

        return result;
    }

    std::string Bytes16x64::ToString() const {
        std::stringstream ss{};

        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 64; j++) {
                ss << int(rows[i].cols[j]) << ' ';
            }
            ss << '\n';
        }

        return ss.str();
    }
}