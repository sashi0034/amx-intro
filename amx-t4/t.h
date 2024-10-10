#include <cstdint>

// No longer used functions

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

// Print int32_t buffer
static void print_dword8x8(const Dword8x8 *buf) {
    for (int i = 0; i < MAX_ROWS_8; i++) {
        for (int j = 0; j < MAX_ROWS_8; j++) {
            printf("%d ", buf->rows[i].cols[j]);
        }

        printf("\n");
    }

    printf("\n");
}
