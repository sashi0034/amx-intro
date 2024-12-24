#include <immintrin.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdbool.h>
#include <memory.h>

#define MAX_SRC_256 256
#define MAX_DST_64 64
#define MAX_ROWS_8 8
#define MAX_COLS_32 32
#define DWORD_SIZE_4 4

#define ARCH_GET_XCOMP_PERM     0x1022
#define ARCH_REQ_XCOMP_PERM     0x1023
#define XFEATURE_XTILECFG       17
#define XFEATURE_XTILEDATA      18

//Define tile config data structure
typedef struct tile_config_t {
    uint8_t palette_id; // 0
    uint8_t start_row; // 1
    uint8_t reserved_2_15[14]; // 2-15: must be zero
    uint16_t colsb[8]; // 16-31
    uint8_t reserved_32_47[16]; // 32-47: must be zero
    uint8_t rows[8]; // 48-55
    uint8_t reserved_56_63[16]; // 56-63: must be zero
} tile_config_t;

typedef struct {
    union {
        struct {
            int8_t cols[32];
        } rows[8];
        int8_t bytes[8 * 32];
    };
} bytes8x32_t;

typedef struct {
    union {
        struct {
            int32_t cols[8];
        } rows[8];
        int32_t dwords[8 * 8];
    };
} dword8x8_t;

typedef struct {
    bytes8x32_t a;
    bytes8x32_t b;
    dword8x8_t c;
} matrix_set_t;

// Initialize tile config
static void init_tile_config_t() {
    tile_config_t tile = {0};
    tile.palette_id = 1;
    tile.start_row = 0;

    // config for dword8x8_t
    tile.colsb[1] = 8 * sizeof(int32_t);
    tile.rows[1] = 8;

    // config for bytes8x32_t
    tile.colsb[2] = 32 * sizeof(int8_t);
    tile.rows[2] = 8;

    // config for bytes8x32_t
    tile.colsb[3] = 32 * sizeof(int8_t);
    tile.rows[3] = 8;

    _tile_loadconfig(&tile);
}

// Invoke syscall to set ARCH_SET_STATE_USE
static bool syscall_for_amx() {
    if (syscall(SYS_arch_prctl, ARCH_REQ_XCOMP_PERM, XFEATURE_XTILEDATA)) {
        printf("\n Fail to do XFEATURE_XTILEDATA \n\n");
        fflush(stdout);
        return false;
    } else {
        return true;
    }
}

typedef struct {
    int64_t cases;
    int8_t *input_buffer;
    int32_t *result_buffer;
} test_buffer_t;

static bool read_test_buffer(test_buffer_t *test_buffer) {
    FILE *file = fopen("test.bin", "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return false;
    }

    fread(&test_buffer->cases, sizeof(test_buffer->cases), 1, file);
    printf("Test cases: %ld\n", test_buffer->cases);

    test_buffer->input_buffer = (int8_t *) malloc(test_buffer->cases * (MAX_SRC_256 * 2) * sizeof(int8_t));
    fread(test_buffer->input_buffer, sizeof(int8_t), test_buffer->cases * (MAX_SRC_256 * 2), file);

    test_buffer->result_buffer = (int32_t *) malloc(test_buffer->cases * (MAX_DST_64) * sizeof(int32_t));
    fread(test_buffer->result_buffer, sizeof(int32_t), test_buffer->cases * (MAX_DST_64), file);

    fclose(file);
    return true;
}

static void init_all_tests_from_buffer(matrix_set_t *test_array, test_buffer_t *buffer) {
    for (int t = 0; t < buffer->cases; ++t) {
        matrix_set_t *mat = &test_array[t];
        bytes8x32_t *a = &mat->a;
        bytes8x32_t *b = &mat->b;

        for (int i = 0; i < MAX_SRC_256; ++i) {
            a->bytes[i] = *(buffer->input_buffer++);
        }

        for (int r = 0; r < MAX_COLS_32; r++) {
            for (int c = 0; c < MAX_ROWS_8; ++c) {
                // Note: The other matrix used in the AMX matrix product calculation is an irregular arrangement
                b->rows[r / 4].cols[c * 4 + r % 4] = *(buffer->input_buffer++);
            }
        }

        memset(&mat->c, 0, sizeof(dword8x8_t));
    }
}

void compute_dot_products(matrix_set_t *test_array, int64_t cases) {
    for (int t = 0; t < cases; ++t) {
        matrix_set_t *mat = &test_array[t];
        _tile_loadd(2, mat->a.bytes, 32 * sizeof(int8_t));
        _tile_loadd(3, mat->b.bytes, 32 * sizeof(int8_t));
        _tile_loadd(1, mat->c.dwords, 8 * sizeof(int32_t));

        // Compute dot-product of bytes in tiles
        _tile_dpbssd(1, 2, 3);

        // Store the tile data to memory
        _tile_stored(1, mat->c.dwords, 8 * sizeof(int32_t));
    }
}

static void check_result_validation(const matrix_set_t *test_array, test_buffer_t *buffer) {
    int errors = 0;
    for (int t = 0; t < (*buffer).cases; ++t) {
        const matrix_set_t *mat = &test_array[t];

        for (int i = 0; i < MAX_DST_64; ++i) {
            int32_t r = *((*buffer).result_buffer++);
            if (mat->c.dwords[i] != r) errors++;
        }
    }

    printf("Errors: %d\n", errors);
}

int main() {
    test_buffer_t buffer;
    if (!read_test_buffer(&buffer)) {
        printf("Failed to read test buffer\n");
        return -1;
    }

    // Allocate memory for test cases
    matrix_set_t *test_array = (matrix_set_t *) malloc(buffer.cases * sizeof(matrix_set_t));

    // Request permission to linux kernel to run AMX
    if (!syscall_for_amx()) return -1;

    // Load tile configuration
    init_tile_config_t();

    // Load test cases for matrix product
    init_all_tests_from_buffer(test_array, &buffer);

    // Compute dot product for each test case
    compute_dot_products(test_array, buffer.cases);

    // // Check if the result is correct
    check_result_validation(test_array, &buffer);

    // Release the tile configuration to return to the init state, which releases all storage it currently holds
    _tile_release();

    free(test_array);

    return 0;
}

