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
typedef struct __tile_config {
    uint8_t palette_id;
    uint8_t start_row;
    uint8_t reserved_0[14];
    uint16_t colsb[16];
    uint8_t rows[16];
} __tilecfg;

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

// Initialize tile config
static void init_tile_config(__tilecfg *tileinfo) {
    int i;
    tileinfo->palette_id = 1;
    tileinfo->start_row = 0;

    tileinfo->colsb[1] = MAX_ROWS_8 * DWORD_SIZE_4;
    tileinfo->rows[1] = MAX_ROWS_8;

    tileinfo->colsb[2] = MAX_COLS_32;
    tileinfo->rows[2] = MAX_ROWS_8;

    tileinfo->colsb[3] = MAX_COLS_32;
    tileinfo->rows[3] = MAX_ROWS_8;

    printf("Tile load start...\n");
    fflush(stdout);

    _tile_loadconfig(tileinfo);

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

    __tilecfg tile_data = {0};

    Bytes8x32 src1;
    Bytes8x32 src2;

    Dword8x8 res;

    // Request permission to linux kernel to run AMX
    if (!set_tiledata_use())
        exit(-1);

    // Load tile configuration
    init_tile_config(&tile_data);

    // Init src matrix buffers with data
    for (int i = 0; i < MAX_SRC_256; ++i) {
        scanf("%hhd", &src1.bytes[i]);
    }

    for (int r = 0; r < MAX_COLS_32; r++) {
        for (int c = 0; c < MAX_ROWS_8; ++c) {
            // Note: The other matrix used in the AMX matrix product calculation is an irregular arrangement
            scanf("%hhd", &src2.rows[r / 4].cols[c * 4 + r % 4]);
        }
    }

    print_bytes8x32(&src1);
    print_bytes8x32(&src2);

    // Init dst matrix buffers with data
    init_dword8x8(&res, 0);

    // Load tile rows from memory
    _tile_loadd(2, src1.bytes, STRIDE_32);
    _tile_loadd(3, src2.bytes, STRIDE_32);
    _tile_loadd(1, res.dwords, STRIDE_32);

    // Compute dot-product of bytes in tiles
    _tile_dpbssd(1, 2, 3);

    // Store the tile data to memory
    _tile_stored(1, res.dwords, STRIDE_32);

    printf("======== amx\n");
    print_dword8x8(&res);

    int errors = 0;
    for (int i = 0; i < MAX_DST_64; ++i) {
        int32_t s;
        scanf("%d", &s);
        if (res.dwords[i] != s) errors++;
    }

    printf("errors: %d\n", errors);

    printf("======== no-amx\n");
    naive_dpb(&res, &src1, &src2);
    print_dword8x8(&res);

    // Release the tile configuration to return to the init state,
    // which releases all storage it currently holds
    _tile_release();
}

