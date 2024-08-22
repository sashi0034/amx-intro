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

#define MAX 1024
#define MAX_ROWS 8
#define MAX_COLS 32
#define STRIDE 32
#define ARCH_GET_XCOMP_PERM     0x1022
#define ARCH_REQ_XCOMP_PERM     0x1023
#define XFEATURE_XTILECFG       17
#define XFEATURE_XTILEDATA      18

//Define tile config data structure
typedef struct __tile_config {
    uint8_t palette_id;
    uint8_t start_row;
    uint8_t reserved_0[14];
    uint16_t colsb[16];
    uint8_t rows[16];
} __tilecfg;

/* Initialize tile config */
static void init_tile_config(__tilecfg *tileinfo) {
    int i;
    tileinfo->palette_id = 1;
    tileinfo->start_row = 0;

    tileinfo->colsb[1] = MAX_COLS;
    tileinfo->rows[1] = MAX_ROWS;

    tileinfo->colsb[2] = MAX_COLS;
    tileinfo->rows[2] = MAX_ROWS;

    tileinfo->colsb[3] = MAX_COLS;
    tileinfo->rows[3] = MAX_ROWS;

//    tileinfo->colsb[3] = MAX_ROWS;
//    tileinfo->rows[3] = MAX_COLS;

    printf("Tile load start...\n");
    fflush(stdout);

    _tile_loadconfig(tileinfo);

    printf("Tile load end\n");
    fflush(stdout);
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

static void naive_dpb(int32_t *dst, int8_t *a, int8_t *b) {
    for (int m = 0; m < MAX_ROWS; ++m) {
        for (int n = 0; n < MAX_ROWS; ++n) {
            const int dst_id = m * MAX_ROWS + n;
            dst[dst_id] = 0;

            for (int k = 0; k < MAX_COLS / 4; ++k) { // [0, 7]
                dst[dst_id] += a[m * MAX_COLS + k * 4 + 0] * b[k * MAX_COLS + n * 4 + 0];
                dst[dst_id] += a[m * MAX_COLS + k * 4 + 1] * b[k * MAX_COLS + n * 4 + 1];
                dst[dst_id] += a[m * MAX_COLS + k * 4 + 2] * b[k * MAX_COLS + n * 4 + 2];
                dst[dst_id] += a[m * MAX_COLS + k * 4 + 3] * b[k * MAX_COLS + n * 4 + 3];
            }
        }
    }


    //    for (int r = 0; r < MAX_ROWS; r++) {
    //        for (int c = 0; c < MAX_ROWS; c++) {
    //            dst[r * MAX_ROWS + c] = 0;
    //
    //            for (int i = 0; i < MAX_COLS; i++) {
    //                dst[r * MAX_ROWS + c] += a[r * MAX_COLS + i] * b[i * MAX_ROWS + c];
    //            }
    //        }
    //    }
}

/* Set_tiledata_use() - Invoke syscall to set ARCH_SET_STATE_USE */
static bool set_tiledata_use() {
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
static void print_buffer(int8_t *buf, int32_t rows, int32_t colsb) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < (colsb); j++) {
            printf("%d ", buf[i * colsb + j]);
        }
        printf("\n");
    }
    printf("\n");
}

static void print_buffer_t(int8_t *buf, int32_t rows, int32_t colsb) {
    for (int j = 0; j < (colsb); j++) {
        for (int i = 0; i < rows; i++) {
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

    for (int i = 0; i < MAX; ++i) {
        src1[i] = i;
        src2[i] = i;
    }

//    for (int i = 0; i < 25; ++i) {
//        src1[i] = 1;
//        src2[i] = 1;
//    }

    //    src1[1 * MAX_COLS + 1] = 1;
    //    src1[2 * MAX_COLS + 25] = 1;

//    src1[14] = 100;
//
//    src2[3 * MAX_COLS + 2] = 10;
//    src2[4 * MAX_COLS + 1] = 1;
//    src2[1 * MAX_COLS + 1] = 1;
//    src2[7 * MAX_COLS + 1] = 1;

    print_buffer(src1, rows, colsb);
    // print_buffer(src2, colsb, rows);

    // Init dst matrix buffers with data
    init_buffer32(res, 0);

    // Load tile rows from memory
    _tile_loadd(2, src1, STRIDE);
    _tile_loadd(3, src2, STRIDE);
    _tile_loadd(1, res, STRIDE);

    // Compute dot-product of bytes in tiles
    _tile_dpbssd(1, 2, 3);

    // Store the tile data to memory
    _tile_stored(1, res, STRIDE);

    printf("======== amx\n");
    print_buffer32(res, rows, colsb / 4);

    printf("======== no-amx\n");
    naive_dpb(res, src1, src2);
    print_buffer32(res, rows, colsb / 4);

    // Release the tile configuration to return to the init state,
    // which releases all storage it currently holds
    _tile_release();
}
