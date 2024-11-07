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

typedef uint16_t bf16_t;

typedef float fp32_t;

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

typedef struct FP32_V8 {
    fp32_t cols[8];
} FP32_V8;

typedef struct FP32_8x8 {
    union {
        FP32_V8 rows[8];
        fp32_t fp32s[8 * 8];
    };
} FP32_8x8;

typedef struct {
    BF16_8x16 a;
    BF16_8x16 b;
    FP32_8x8 c;
} MatrixTuple;

// -----------------------------------------------
// Test Matrix
typedef struct FP32_V16 {
    fp32_t cols[16];
} FP32_V16;

typedef struct FP32_8x16 {
    union {
        FP32_V16 rows[8];
        fp32_t fp32s[8 * 16];
    };
} FP32_8x16;

typedef struct FP32_16x8 {
    union {
        FP32_V8 rows[16];
        fp32_t fp32s[16 * 8];
    };
} FP32_16x8;

void randomize_fp32_128(fp32_t fp32s[restrict SRC_ELEMS_128]) {
    for (int i = 0; i < SRC_ELEMS_128; ++i) {
        fp32s[i] = ((fp32_t) rand()) / RAND_MAX;
    }
}

void dp_8x16_16x8(FP32_8x8 *c, const FP32_8x16 *a, const FP32_16x8 *b) {
    for (int row = 0; row < DST_ROWS_8; ++row) {
        for (int col = 0; col < DST_COLS_8; ++col) {
            c->rows[row].cols[col] = 0.0f;
            for (int i = 0; i < SRC_COLS_16; ++i) {
                c->rows[row].cols[col] += a->rows[row].cols[i] * b->rows[i].cols[col];
            }
        }
    }
}

void print_fp32_8x16(const FP32_8x16 *mat) {
    for (int i = 0; i < SRC_ROWS_8; i++) {
        for (int j = 0; j < SRC_COLS_16; j++) {
            printf("%f ", mat->rows[i].cols[j]);
        }
        printf("\n");
    }
}

void print_fp32_16x8(const FP32_16x8 *mat) {
    for (int i = 0; i < SRC_COLS_16; i++) {
        for (int j = 0; j < SRC_ROWS_8; j++) {
            printf("%f ", mat->rows[i].cols[j]);
        }
        printf("\n");
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

static void init_bf16_8x16(BF16_8x16 *mat, bf16_t value) {
    for (int i = 0; i < SRC_ROWS_8; i++) {
        for (int j = 0; j < SRC_COLS_16; j++) {
            mat->rows[i].cols[j] = value;
        }
    }
}

static void init_fp32_8x8(FP32_8x8 *mat, fp32_t value) {
    for (int i = 0; i < DST_ROWS_8; i++) {
        for (int j = 0; j < DST_COLS_8; j++) {
            mat->rows[i].cols[j] = value;
        }
    }
}

static void print_bf16_8x16(const BF16_8x16 *mat) {
    for (int i = 0; i < SRC_ROWS_8; i++) {
        for (int j = 0; j < SRC_COLS_16; j++) {
            printf("%f ", bf16_to_fp32(mat->rows[i].cols[j]));
        }
        printf("\n");
    }
}

static void print_fp32_8x8(const FP32_8x8 *mat) {
    for (int i = 0; i < DST_ROWS_8; i++) {
        for (int j = 0; j < DST_COLS_8; j++) {
            printf("%f ", mat->rows[i].cols[j]);
        }
        printf("\n");
    }
}

void amx_dot_product(MatrixTuple *mat) {
    _tile_loadd(2, mat->a.bf16s, STRIDE_32);
    _tile_loadd(3, mat->b.bf16s, STRIDE_32);
    _tile_loadd(1, mat->c.fp32s, STRIDE_32);

    // Compute dot-product of bytes in tiles
    // https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#amxtechs=AMX_BF16&ig_expand=6864
    _tile_dpbf16ps(1, 2, 3);

    // Store the tile data to memory
    _tile_stored(1, mat->c.fp32s, STRIDE_32);

    // Add a compiler memory barrier to prevent optimization issues
    __asm__ __volatile__ ("" : "+m" (mat->c.fp32s));
}

int main() {
    tile_config tile_data = {0};

    // Request permission to linux kernel to run AMX
    if (!set_tiledata_use())
        exit(-1);

    MatrixTuple amx_matrix;

    // -----------------------------------------------

    FP32_8x16 a;
    FP32_16x8 b;
    FP32_8x8 c;

    randomize_fp32_128(a.fp32s);
    randomize_fp32_128(b.fp32s);
    dp_8x16_16x8(&c, &a, &b);

    printf("-----------------------------------------------\n");
    printf("Origin A:\n");
    print_fp32_8x16(&a);

    printf("-----------------------------------------------\n");
    printf("Origin B:\n");
    print_fp32_16x8(&b);

    printf("-----------------------------------------------\n");
    printf("Origin C:\n");
    print_fp32_8x8(&c);

    fp32_8x16_to_bf16_8x16(&amx_matrix.a, &a);
    fp32_16x8_to_bf16_8x16(&amx_matrix.b, &b);
    // -----------------------------------------------

    init_fp32_8x8(&amx_matrix.c, 0.0f);

    // Load tile configuration
    init_tile_config(&tile_data);

    printf("-----------------------------------------------\n");
    printf("Matrix A:\n");
    print_bf16_8x16(&amx_matrix.a);

    printf("-----------------------------------------------\n");
    printf("Matrix B:\n");
    print_bf16_8x16(&amx_matrix.b);

    // Compute dot product for each test case
    amx_dot_product(&amx_matrix);

    printf("-----------------------------------------------\n");
    printf("Result matrix:\n");
    print_fp32_8x8(&amx_matrix.c);

    // Release the tile configuration to return to the init state, which releases all storage it currently holds
    _tile_release();

    printf("-----------------------------------------------\n");
    printf("Finished task.\n");

    return 0;
}

