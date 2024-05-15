//
// Created by b38415 on 5/15/24.
//

#ifndef AMX_INTRO_MATRIX_H
#define AMX_INTRO_MATRIX_H


#include <cstdint>

namespace amx {
    struct Byte16x64 {
        union {
            int8_t bytes[16 * 64];
            struct {
                int8_t rows[64];
            } cols[16];
        };
    };

    struct Ward16x16 {
        union {
            int32_t bytes[16 * 16];
            struct {
                int32_t rows[16];
            } cols[16];
        };
    };
}


#endif //AMX_INTRO_MATRIX_H
