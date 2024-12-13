#include <immintrin.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define MAX_SRC_256 256
#define MAX_DST_64 64
#define MAX_ROWS_8 8
#define MAX_COLS_32 32
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
    Bytes8x32 input;
    Dword8x8 output;
} MatrixTuple;

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

static void init_dword8x8(Dword8x8 *mat, int32_t value) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            mat->rows[i].cols[j] = value;
        }
    }
}

typedef struct {
    int64_t cases;
    int8_t *filter_buffer;
    int8_t *input_buffer;
    int32_t *result_buffer;
} TestBuffer;

static bool read_test_buffer(TestBuffer *test_buffer) {
    FILE *file = fopen("test.bin", "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return false;
    }

    fread(&test_buffer->cases, sizeof(test_buffer->cases), 1, file);
    printf("Test cases: %ld\n", test_buffer->cases);

    test_buffer->filter_buffer = (int8_t *) malloc(1 * (MAX_SRC_256) * sizeof(int8_t));
    fread(test_buffer->filter_buffer, sizeof(int8_t), 1 * (MAX_SRC_256), file);

    test_buffer->input_buffer = (int8_t *) malloc(test_buffer->cases * (MAX_SRC_256) * sizeof(int8_t));
    fread(test_buffer->input_buffer, sizeof(int8_t), test_buffer->cases * (MAX_SRC_256), file);

    test_buffer->result_buffer = (int32_t *) malloc(test_buffer->cases * (MAX_DST_64) * sizeof(int32_t));
    fread(test_buffer->result_buffer, sizeof(int32_t), test_buffer->cases * (MAX_DST_64), file);

    fclose(file);
    return true;
}

static void init_all_tests_from_buffer(MatrixTuple *test_array, Bytes8x32 *filter, TestBuffer *buffer) {
    for (int t = 0; t < buffer->cases; ++t) {
        MatrixTuple *mat = &test_array[t];

        for (int i = 0; i < MAX_SRC_256; ++i) {
            mat->input.bytes[i] = *(buffer->input_buffer++);
        }

        init_dword8x8(&mat->output, 0);
    }

    for (int r = 0; r < MAX_COLS_32; r++) {
        for (int c = 0; c < MAX_ROWS_8; ++c) {
            // Note: The other matrix used in the AMX matrix product calculation is an irregular arrangement
            filter->rows[r / 4].cols[c * 4 + r % 4] = *(buffer->filter_buffer++);
        }
    }
}

// Initialize tile config
static void init_tile_config(tile_config *tile) {
    tile->palette_id = 1;
    tile->start_row = 0;

    tile->colsb[1] = MAX_COLS_32 * sizeof(int8_t);
    tile->rows[1] = MAX_ROWS_8;

    tile->colsb[2] = MAX_COLS_32 * sizeof(int8_t);
    tile->rows[2] = MAX_ROWS_8;

    tile->colsb[3] = MAX_ROWS_8 * sizeof(int32_t);
    tile->rows[3] = MAX_ROWS_8;

    AMX_LOG("Tile load start...\n");

    _tile_loadconfig(tile);

    AMX_LOG("Tile load end\n");
}

void compute_all_tests(MatrixTuple *test_array, Bytes8x32 *filter, int64_t cases) {
    _tile_loadd(1, filter->bytes, STRIDE_32);

    for (int64_t t = 0; t < cases; ++t) {
        MatrixTuple *mat = &test_array[t];
        _tile_loadd(2, mat->input.bytes, STRIDE_32);
        _tile_loadd(3, mat->output.dwords, STRIDE_32);

        // Compute dot-product of bytes in tiles
        _tile_dpbssd(3, 2, 1);

        // Store the tile data to memory
        _tile_stored(3, mat->output.dwords, STRIDE_32);
    }
}

static void check_result_validation(const MatrixTuple *test_array, TestBuffer *buffer) {
    int errors = 0;
    for (int t = 0; t < (*buffer).cases; ++t) {
        const MatrixTuple *mat = &test_array[t];

        for (int i = 0; i < MAX_DST_64; ++i) {
            int32_t r = *((*buffer).result_buffer++);
            if (mat->output.dwords[i] != r) errors++;
        }
    }

    printf("Errors: %d\n", errors);
}

static void stamp_time_used(clock_t start, const char *message) {
    clock_t end = clock();
    double time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%s: %f\n", message, time_used);
}

int main() {
    TestBuffer buffer;
    if (!read_test_buffer(&buffer)) {
        printf("Failed to read test buffer\n");
        return -1;
    }

    tile_config tile_data = {0};

    // Allocate memory for test cases
    MatrixTuple *test_array = (MatrixTuple *) malloc(buffer.cases * sizeof(MatrixTuple));
    Bytes8x32 test_filter;

    // Request permission to linux kernel to run AMX
    if (!set_tiledata_use())
        exit(-1);

    // Load tile configuration
    init_tile_config(&tile_data);

    // Load test cases for matrix product
    init_all_tests_from_buffer(test_array, &test_filter, &buffer);

    // Compute dot product for each test case
    const clock_t compute_started = clock();
    compute_all_tests(test_array, &test_filter, buffer.cases);
    stamp_time_used(compute_started, "Compute time");

    // // Check if the result is correct
    check_result_validation(test_array, &buffer);

    // Release the tile configuration to return to the init state, which releases all storage it currently holds
    _tile_release();

    free(test_array);

    return 0;
}

