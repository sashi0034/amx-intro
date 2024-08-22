//==============================================================
// Copyright © 2022 Intel Corporation
//
// SPDX-License-Identifier: MIT
// =============================================================
#include <immintrin.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX 1024 // 16 * 64
#define MAX_ROWS 16
#define MAX_COLS 64
#define STRIDE 64
#define ARCH_GET_XCOMP_PERM 0x1022
#define ARCH_REQ_XCOMP_PERM 0x1023
#define XFEATURE_XTILECFG 17
#define XFEATURE_XTILEDATA 18

// Define tile config data structure
typedef struct __tile_config {
    // 0: palette_id
    uint8_t palette_id;

    // 1: startRow (8b)
    uint8_t start_row;

    // 2-15: reserved (must be zero)
    uint8_t reserved_0[14];

    // 16-17: tile0.colsb -- bytes_per_row
    // 18-19: tile1.colsb
    // 20-21: tile2.colsb
    // ...
    // 46-47: tile15.colsb
    uint16_t colsb[16];

    // 48: tile0.rows
    // 49: tile1.rows
    // 50: tile2.rows
    // ...
    // 63: tile15.rows
    uint8_t rows[16];
} __tilecfg;

/* Initialize tile config */
static void init_tile_config(__tilecfg *tileinfo) {
    // パレット 0 は初期化された状態
    // パレット 1 は 8 つのタイル レジスタに分割された 8 KB のストレージで構成され、各タイルの最大サイズは 16 行 x 64 バイト
    tileinfo->palette_id = 1;

    tileinfo->start_row = 0;

    for (int i = 0; i <= 0; ++i) {
        tileinfo->colsb[i] = MAX_ROWS;
        tileinfo->rows[i] = MAX_ROWS;
    }

    for (int i = 1; i <= 3; ++i) {
        tileinfo->colsb[i] = MAX_COLS;
        tileinfo->rows[i] = MAX_ROWS;
    }

    _tile_loadconfig(tileinfo);
}

/* Initialize int8_t buffer */
static void init_buffer(int8_t *buf, int8_t value) {
    int rows, colsb, i, j;
    rows = MAX_ROWS;
    colsb = MAX_COLS;

    for (i = 0; i < rows; i++)
        for (j = 0; j < colsb; j++) {
            buf[i * colsb + j] = value;
        }
}

/* Initialize int32_t buffer */
static void init_buffer32(int32_t *buf, int32_t value) {
    int rows, colsb, i, j;
    rows = MAX_ROWS;
    colsb = MAX_COLS;
    int colsb2 = colsb / 4;

    for (i = 0; i < rows; i++)
        for (j = 0; j < (colsb2); j++) {
            buf[i * colsb2 + j] = value;
        }
}

/* Set_tiledata_use() - Invoke syscall to set ARCH_SET_STATE_USE */
static bool set_tiledata_use() {
    // Linux システムコールを呼び出して、インテル AMX 機能へのアクセスを要求
    if (syscall(SYS_arch_prctl, ARCH_REQ_XCOMP_PERM, XFEATURE_XTILEDATA)) {
        printf("\n Fail to do XFEATURE_XTILEDATA \n\n");
        return false;
    } else {
        printf("\n TILE DATA USE SET - OK \n\n");
        return true;
    }

    return true;
}

/* Print int8_t buffer */
static void print_buffer8(int8_t *buf, int32_t rows, int32_t colsb) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < (colsb); j++) {
            printf("%d ", buf[i * colsb + j]);
        }
        printf("\n");
    }
    printf("\n");
}

/* Print int32_t buffer */
static void print_buffer32(int32_t *buf, int32_t rows, int32_t colsb) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < (colsb); j++) {
            printf("%d ", buf[i * colsb + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    __tilecfg tile_data = {0};
    int8_t src1[MAX];
    int8_t src2[MAX];
    int32_t res[MAX / 4];
    int rows = MAX_ROWS;
    int colsb = MAX_COLS;

    // Request permission to linux kernel to run AMX
    if (!set_tiledata_use())
        exit(-1);

    // Load tile configuration
    init_tile_config(&tile_data);

    // Init src matrix buffers with data
    init_buffer(src1, 0);

    init_buffer(src2, 0);

    for (int i = 0; i < 20; ++i) {
        src1[i] = 1;
        src2[i] = 1;
    }

    print_buffer8(src1, rows, colsb);

    print_buffer8(src2, rows, colsb);

    // Init dst matrix buffers with data
    init_buffer32(res, 0);

    // Load tile rows from memory
    _tile_loadd(2, src1, STRIDE);
    _tile_loadd(3, src2, STRIDE);
    _tile_loadd(1, res, STRIDE); // 恐らくこの処理は不要

    // Compute dot-product of bytes in tiles
    _tile_dpbssd(1, 2, 3); // (dst, src0, src1)

    // Store the tile data to memory
    _tile_stored(1, res, STRIDE);
    print_buffer32(res, rows, colsb / 4);

    // Release the tile configuration to return to the init state,
    // which releases all storage it currently holds
    _tile_release();
}
