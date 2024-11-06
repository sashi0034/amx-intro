#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <immintrin.h>

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

static void init_dword8x8(Dword8x8 *mat, int32_t value) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            mat->rows[i].cols[j] = value;
        }
    }
}

void compute_all_tests(MatrixTuple *test_array, int cases) {
    for (int t = 0; t < cases; ++t) {
        MatrixTuple *mat = &test_array[t];
        _tile_loadd(2, mat->a.bytes, STRIDE_32);
        _tile_loadd(3, mat->b.bytes, STRIDE_32);
        _tile_loadd(1, mat->c.dwords, STRIDE_32);

        // Compute dot-product of bytes in tiles
        _tile_dpbssd(1, 2, 3);

        // Store the tile data to memory
        _tile_stored(1, mat->c.dwords, STRIDE_32);
    }
}

void bf16_example() {
    // https://www.isus.jp/machine-learning/dl-boost-new-instruction-bfloat16/

    float op1_f32[16];
    float op2_f32[16];
    float op3_f32[16];
    float res_f32[16];
    float res_comp_f32[16];

    // レジスター変数
    __m512 v1_f32;
    __m512 v2_f32;
    __m512 v3_f32;
    __m512bh v1_f16;
    __m512bh v2_f16;
    __m512 vr_f32;

    // 配列のサンプル値を選択
    float v = sqrt(2);
    for (int i = 0; i < 16; i++) {
        op1_f32[i] = v;
        op2_f32[i] = v;
        res_f32[i] = 1.0;

        // float32 を使用したドット積の結果 (bf16 との比較に使用)
        res_comp_f32[i] = 2.0 * op1_f32[i] * op2_f32[i] + res_f32[i];
    }

    // 16 個の float32 値をレジスターにロード
    // (データは特定の境界でアライメントされている必要はありません)
    v1_f32 = _mm512_loadu_ps(op1_f32);
    v2_f32 = _mm512_loadu_ps(op2_f32);
    vr_f32 = _mm512_loadu_ps(res_f32);

    // 2 つの float32 レジスター (それぞれ 16 個の値を格納) を1 つの BF16 レジスター #1 (32 個の値) に変換
    v1_f16 = _mm512_cvtne2ps_pbh(v1_f32, v2_f32);

    // 2 つの float32 レジスター (それぞれ 16 個の値を格納) を 1 つの BF16 レジスター #2 (32 個の値) に変換
    v2_f16 = _mm512_cvtne2ps_pbh(v1_f32, v2_f32);

    // FMA: BF16 レジスター #1 と #2 のドット積を計算
    // 結果を 1 つの float32 レジスターに累積
    vr_f32 = _mm512_dpbf16_ps(vr_f32, v1_f16, v2_f16);

    // レジスターの値をメモリーにコピー
    // (メモリーアドレスは特定の境界にアライメントされている必要はありません)
    _mm512_storeu_ps((void *) res_f32, vr_f32);
}

int main() {
    tile_config tile_data = {0};

    MatrixTuple test_matrix;

    // Request permission to linux kernel to run AMX
    if (!set_tiledata_use())
        exit(-1);

    // Load tile configuration
    init_tile_config(&tile_data);

    // Compute dot product for each test case
    // compute_all_tests(test_array, buffer.cases);

    // Release the tile configuration to return to the init state, which releases all storage it currently holds
    _tile_release();

    printf("Finished task.\n");

    return 0;
}

