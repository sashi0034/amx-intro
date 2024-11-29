#ifndef AMX_INTRO_T9_AMX_8X16_H
#define AMX_INTRO_T9_AMX_8X16_H

#include <stdint.h>

typedef uint16_t bf16_t;

typedef float fp32_t;

typedef struct FP32_V8 {
    fp32_t cols[8];
} FP32_V8;

typedef struct FP32_V16 {
    fp32_t cols[16];
} FP32_V16;

typedef struct FP32_8x16 {
    union {
        FP32_V16 rows[8];
        fp32_t fp32s[8 * 16];
    };
} FP32_8x16;

typedef struct FP32_8x8 {
    union {
        FP32_V8 rows[8];
        fp32_t fp32s[8 * 8];
    };
} FP32_8x8;

typedef struct FP32_16x8 {
    union {
        FP32_V8 rows[16];
        fp32_t fp32s[16 * 8];
    };
} FP32_16x8;

void init_amx_8x16();

void amx_dp_8x16_16x8(FP32_8x8 *c, const FP32_8x16 *a, const FP32_16x8 *b);

void release_amx();

#endif //AMX_INTRO_T9_AMX_8X16_H
