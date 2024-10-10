#include <immintrin.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_SRC_256 256
#define MAX_DST_64 64
#define MAX_ROWS_8 8
#define MAX_COLS_32 32
#define STRIDE_32 32
#define DWORD_SIZE_4 4

#define ARCH_GET_XCOMP_PERM     0x1022
#define ARCH_REQ_XCOMP_PERM     0x1023
#define XFEATURE_XTILECFG       17
#define XFEATURE_XTILEDATA      18

//Define tile config data structure
typedef struct { // __tile_config
    uint8_t palette_id;
    uint8_t start_row;
    uint8_t reserved_0[14];
    uint16_t colsb[16];
    uint8_t rows[16];
} tile_config;

typedef struct {
    int8_t cols[32];
} Bytes32;

typedef struct {
    union {
        Bytes32 rows[8];
        int8_t bytes[32 * 8];
    };
} Bytes8x32;

typedef struct {
    int32_t cols[8];
} Dword8;

typedef struct {
    union {
        Dword8 rows[8];
        int32_t dwords[8 * 8];
    };
} Dword8x8;

typedef struct {
    Bytes8x32 a;
    Bytes8x32 b;
    Dword8x8 c;
} MatrixTuple;

// Initialize tile config
static void init_tile_config(tile_config *tile) {
    int i;
    tile->palette_id = 1;
    tile->start_row = 0;

    tile->colsb[1] = MAX_ROWS_8 * DWORD_SIZE_4;
    tile->rows[1] = MAX_ROWS_8;

    tile->colsb[2] = MAX_COLS_32;
    tile->rows[2] = MAX_ROWS_8;

    tile->colsb[3] = MAX_COLS_32;
    tile->rows[3] = MAX_ROWS_8;

    printf("Tile load start...\n");
    fflush(stdout);

    _tile_loadconfig(tile);

    printf("Tile load end\n");
    fflush(stdout);
}

// Initialize 8x32 bytes buffer
static void init_bytes8x32(Bytes8x32 *buf, int8_t value) {
    for (int i = 0; i < MAX_ROWS_8; i++) {
        for (int j = 0; j < MAX_COLS_32; j++) {
            buf->rows[i].cols[j] = value;
        }
    }
}

// Initialize 8x8 dword buffer
static void init_dword8x8(Dword8x8 *buf, int32_t value) {
    for (int i = 0; i < MAX_ROWS_8; i++) {
        for (int j = 0; j < MAX_ROWS_8; j++) {
            buf->rows[i].cols[j] = value;
        }
    }
}

static void naive_dpb(Dword8x8 *dst, Bytes8x32 *a, Bytes8x32 *b) {
    for (int m = 0; m < MAX_ROWS_8; ++m) {
        for (int n = 0; n < MAX_ROWS_8; ++n) {
            dst->rows[m].cols[n] = 0;

            for (int k = 0; k < MAX_COLS_32 / 4; ++k) { // [0, 7]
                dst->rows[m].cols[n] += a->rows[m].cols[k * 4 + 0] * b->rows[k].cols[n * 4 + 0];
                dst->rows[m].cols[n] += a->rows[m].cols[k * 4 + 1] * b->rows[k].cols[n * 4 + 1];
                dst->rows[m].cols[n] += a->rows[m].cols[k * 4 + 2] * b->rows[k].cols[n * 4 + 2];
                dst->rows[m].cols[n] += a->rows[m].cols[k * 4 + 3] * b->rows[k].cols[n * 4 + 3];
            }
        }
    }
}

// Set_tiledata_use() - Invoke syscall to set ARCH_SET_STATE_USE
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

// Print int8_t buffer
static void print_bytes8x32(const Bytes8x32 *buf) {
    for (int i = 0; i < MAX_ROWS_8; i++) {
        for (int j = 0; j < MAX_COLS_32; j++) {
            printf("%d ", buf->rows[i].cols[j]);
        }

        printf("\n");
    }

    printf("\n");
}

/* Print int32_t buffer */
static void print_dword8x8(const Dword8x8 *buf) {
    for (int i = 0; i < MAX_ROWS_8; i++) {
        for (int j = 0; j < MAX_ROWS_8; j++) {
            printf("%d ", buf->rows[i].cols[j]);
        }

        printf("\n");
    }

    printf("\n");
}

int main() {
    int test_cases;
    scanf("%d", &test_cases);
    printf("Test cases: %d\n", test_cases);

    tile_config tile_data = {0};

    // Allocate memory for test cases
    MatrixTuple *test_array = (MatrixTuple *) malloc(test_cases * sizeof(MatrixTuple));

    // Request permission to linux kernel to run AMX
    if (!set_tiledata_use())
        exit(-1);

    // Load tile configuration
    init_tile_config(&tile_data);

    // Load test cases for matrix product
    for (int t = 0; t < test_cases; ++t) {
        MatrixTuple *test = &test_array[t];
        Bytes8x32 *a = &test->a;
        Bytes8x32 *b = &test->b;
        for (int i = 0; i < MAX_SRC_256; ++i) {
            scanf("%hhd", &a->bytes[i]);
        }

        for (int r = 0; r < MAX_COLS_32; r++) {
            for (int c = 0; c < MAX_ROWS_8; ++c) {
                // Note: The other matrix used in the AMX matrix product calculation is an irregular arrangement
                scanf("%hhd", &b->rows[r / 4].cols[c * 4 + r % 4]);
            }
        }

        init_dword8x8(&test->c, 0);
    }

    for (int t = 0; t < test_cases; ++t) {
        // Load tile rows from memory
        MatrixTuple *test = &test_array[t];
        _tile_loadd(2, test->a.bytes, STRIDE_32);
        _tile_loadd(3, test->b.bytes, STRIDE_32);
        _tile_loadd(1, test->c.dwords, STRIDE_32);

        // Compute dot-product of bytes in tiles
        _tile_dpbssd(1, 2, 3);

        // Store the tile data to memory
        _tile_stored(1, test->c.dwords, STRIDE_32);
    }

    int errors = 0;
    for (int t = 0; t < test_cases; ++t) {
        // Load tile rows from memory
        MatrixTuple *test = &test_array[t];

        for (int i = 0; i < MAX_DST_64; ++i) {
            int32_t s;
            scanf("%d", &s);
            if (test->c.dwords[i] != s) errors++;
        }
    }

    printf("Errors: %d\n", errors);

//    naive_dpb(&res, &src1, &src2);
//    print_dword8x8(&res);

    // Release the tile configuration to return to the init state,
    // which releases all storage it currently holds
    _tile_release();

    free(test_array);

    return 0;
}

