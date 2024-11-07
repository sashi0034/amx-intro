// https://www.isus.jp/machine-learning/dl-boost-new-instruction-bfloat16/

//==============================================================
// Copyright © 2019 Intel Corporation
//
// SPDX-License-Identifier: MIT
// =============================================================

#include <immintrin.h>
#include <stdio.h>
#include <math.h>

int main() {

    float op1_f32[16];
    float op2_f32[16];
    float op3_f32[16];
    float res_f32[16];
    float res_comp_f32[16];

    // register variables
    __m512 v1_f32;
    __m512 v2_f32;
    __m512 v3_f32;
    __m512bh v1_f16;
    __m512bh v2_f16;
    __m512 vr_f32;

    // Choose some sample values for arrays
    float v = sqrt(2);
    for (int i = 0; i < 16; i++) {
        op1_f32[i] = v + i;
        op2_f32[i] = v + i;
        // op3_f32[i] = 0.0;
        res_f32[i] = 1.0;
    }

    for (int i = 0; i < 16; i++) {
        // Compute result of dot product operation using float32 (for comparison with bf16)
        res_comp_f32[i] = res_f32[i];
        const int t = 2 * i < 16 ? (2 * i) : (2 * i - 16);
        res_comp_f32[i] += op1_f32[t + 1] * op2_f32[t + 1];
        res_comp_f32[i] += op1_f32[t] * op2_f32[t];
    }

    // Display input values
    printf("\nINPUT TO BF16 INSTRUCTION: \n");
    printf("  Two float32 vectors, each containing values: \n");
    for (int j = 15; j >= 0; j--) {
        printf("  %f ", op1_f32[j]);
    }
    printf("\n\n");
    printf("  One float32 vector (input/output vector), containing values: \n");
    for (int j = 15; j >= 0; j--) {
        printf("  %f ", res_f32[j]);
    }
    printf("\n");

    // Load 16 float32 values into registers (data does not need to be aligned on any particular boundary)
    v1_f32 = _mm512_loadu_ps(op1_f32);
    v2_f32 = _mm512_loadu_ps(op2_f32);
    // v3_f32 = _mm512_loadu_ps(op3_f32);
    vr_f32 = _mm512_loadu_ps(res_f32);

    // Convert two float32 registers (16 values each) to one BF16 register #1 (32 values)
    v1_f16 = _mm512_cvtne2ps_pbh(v1_f32, v2_f32);

    // Convert two float32 registers (16 values each) to one BF16 register #2 (32 values)
    v2_f16 = _mm512_cvtne2ps_pbh(v1_f32, v2_f32);

    // FMA: Performs dot product of BF16 registers #1 and #2. Accumulate result into one float32 output register
    vr_f32 = _mm512_dpbf16_ps(vr_f32, v1_f16, v2_f16);

    // Copy output register to memory (memory address does not need to be aligned on any particular boundary)
    _mm512_storeu_ps((void *) res_f32, vr_f32);

    // Display results
    printf("\nRESULTS OF DOT PRODUCT USING BF16 INSTRUCTION: \n");
    for (int j = 15; j >= 0; j--) {
        printf("%f ", res_f32[j]);
    }
    printf("\n");

    printf("\nRESULTS OF DOT PRODUCT USING FLOAT32 INSTRUCTIONS : \n");
    for (int j = 15; j >= 0; j--) {
        printf("%f ", res_comp_f32[j]);
    }
    printf("\n\n");

    return 0;
}
