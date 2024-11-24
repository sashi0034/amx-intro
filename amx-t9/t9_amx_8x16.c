#include "t9_amx_8x16.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdbool.h>
#include <immintrin.h>
#include <memory.h>

#define SRC_ELEMS_128 128
#define SRC_ROWS_8 8
#define SRC_COLS_16 16

#define DST_ELEMS_64 64
#define DST_ROWS_8 8
#define DST_COLS_8 8

#define STRIDE_32 32

#define ARCH_GET_XCOMP_PERM     0x1022
#define ARCH_REQ_XCOMP_PERM     0x1023
#define XFEATURE_XTILECFG       17
#define XFEATURE_XTILEDATA      18

// #define ENABLE_AMX_LOG

#ifdef ENABLE_AMX_LOG
#define AMX_LOG(fmt, ...) printf(fmt, ##__VA_ARGS__); fflush(stdout)
#else
#define AMX_LOG(fmt, ...) (void)fmt // Do nothing
#endif

static bf16_t fp32_to_bf16(fp32_t value) {
    const uint16_t v = (*((uint32_t *) &value)) >> 16;
    return v;
}

static fp32_t bf16_to_fp32(bf16_t value) {
    const uint32_t v = ((uint32_t) value) << 16;
    return (*((fp32_t *) &v));
}

//Define tile config data structure
typedef struct { // __tile_config
    uint8_t palette_id;
    uint8_t start_row;
    uint8_t reserved_0[14];
    uint16_t colsb[16];
    uint8_t rows[16];
} tile_config;

typedef struct BF16_V16 {
    bf16_t cols[16];
} BF16_V16;

typedef struct BF16_8x16 {
    union {
        BF16_V16 rows[8];
        bf16_t bf16s[8 * 16];
    };
} BF16_8x16;

// -----------------------------------------------

// Initialize tile config
static void init_tile_config(tile_config *tile) {
    int i;
    tile->palette_id = 1;
    tile->start_row = 0;

    // Result Register
    tile->colsb[1] = DST_COLS_8 * sizeof(fp32_t);
    tile->rows[1] = DST_ROWS_8;

    // Source Register A
    tile->colsb[2] = SRC_COLS_16 * sizeof(bf16_t);
    tile->rows[2] = SRC_ROWS_8;

    // Source Register B
    tile->colsb[3] = SRC_COLS_16 * sizeof(bf16_t);
    tile->rows[3] = SRC_ROWS_8;

    AMX_LOG("Tile load start...\n");

    _tile_loadconfig(tile);

    AMX_LOG("Tile load end\n");
}

// Set_tiledata_use() - Invoke syscall to set ARCH_SET_STATE_USE
static bool set_tiledata_use() {
    if (syscall(SYS_arch_prctl, ARCH_REQ_XCOMP_PERM, XFEATURE_XTILEDATA)) {
        AMX_LOG("\n Fail to do XFEATURE_XTILEDATA \n\n");
        return false;
    } else {
        AMX_LOG("\n TILE DATA USE SET - OK \n\n");
        return true;
    }
}

void fp32_8x16_to_bf16_8x16(BF16_8x16 *bf16s, const FP32_8x16 *fp32s) {
    for (int r = 0; r < SRC_ROWS_8; r++) {
        for (int c = 0; c < SRC_COLS_16; c++) {
            bf16s->rows[r].cols[c] = fp32_to_bf16(fp32s->rows[r].cols[c]);
        }
    }
}

void fp32_16x8_to_bf16_8x16(BF16_8x16 *bf16s, const FP32_16x8 *fp32s) {
    for (int r = 0; r < SRC_COLS_16; r++) {
        for (int c = 0; c < SRC_ROWS_8; c++) {
            bf16s->rows[r / 2].cols[c * 2 + r % 2] = fp32_to_bf16(fp32s->rows[r].cols[c]);
        }
    }
}

static void amx_dot_product(FP32_8x8 *c, BF16_8x16 *a, BF16_8x16 *b) {
    _tile_loadd(2, a->bf16s, STRIDE_32);
    _tile_loadd(3, b->bf16s, STRIDE_32);
    _tile_loadd(1, c->fp32s, STRIDE_32);

    // Compute dot-product of bytes in tiles
    // https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#amxtechs=AMX_BF16&ig_expand=6864
    _tile_dpbf16ps(1, 2, 3);

    // Store the tile data to memory
    _tile_stored(1, c->fp32s, STRIDE_32);

    // Add a compiler memory barrier to prevent optimization issues
    __asm__ __volatile__ ("" : "+m" (c->fp32s));
}

// -----------------------------------------------
// Exported functions

void init_amx_8x16() {
    tile_config tile_data = {0};

    // Request permission to linux kernel to run AMX
    if (!set_tiledata_use())
        exit(-1);

    init_tile_config(&tile_data);
}

void amx_dp_8x16_16x8(FP32_8x8 *c, const FP32_8x16 *a, const FP32_16x8 *b) {
    BF16_8x16 bf16_a;
    BF16_8x16 bf16_b;

    fp32_8x16_to_bf16_8x16(&bf16_a, a);
    fp32_16x8_to_bf16_8x16(&bf16_b, b);

    amx_dot_product(c, &bf16_a, &bf16_b);
}

void release_amx() {
    // Release the tile configuration to return to the init state, which releases all storage it currently holds
    _tile_release();
}


