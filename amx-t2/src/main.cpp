#include <iostream>
#include <vector>
#include <array>
#include <unistd.h>
#include <sys/syscall.h>
#include <cstdint>
#include "AmxCalculator.h"
#include "Matrix.h"

constexpr int MAX = 1024;
constexpr int MAX_ROWS = 16;
constexpr int MAX_COLS = 64;
constexpr int STRIDE = 64;
constexpr int ARCH_GET_XCOMP_PERM = 0x1022;
constexpr int ARCH_REQ_XCOMP_PERM = 0x1023;
constexpr int XFEATURE_XTILECFG = 17;
constexpr int XFEATURE_XTILEDATA = 18;

struct TileConfig {
    uint8_t palette_id = 0;
    uint8_t start_row = 0;
    uint8_t reserved_0[14] = {0};
    std::array<uint16_t, 16> colsb = {0};
    std::array<uint8_t, 16> rows = {0};

    void init() {
        palette_id = 1;
        start_row = 0;
        colsb[0] = MAX_ROWS;
        rows[0] = MAX_ROWS;
        for (int i = 1; i <= 3; ++i) {
            colsb[i] = MAX_COLS;
            rows[i] = MAX_ROWS;
        }
        _tile_loadconfig(this);
    }
};

template<typename T>
void init_buffer(std::vector<T> &buf, T value) {
    std::fill(buf.begin(), buf.end(), value);
}

template<typename T>
void print_buffer(const std::vector<T> &buf, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << static_cast<int>(buf[i * cols + j]) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

bool set_tiledata_use() {
    if (syscall(SYS_arch_prctl, ARCH_REQ_XCOMP_PERM, XFEATURE_XTILEDATA)) {
        std::cerr << "\nFail to set XFEATURE_XTILEDATA\n\n";
        return false;
    }
    std::cout << "\nTILE DATA USE SET - OK\n\n";
    return true;
}

int main() {
    amx::sayHello();

    amx::Bytes16x64 bbb{};
    std::cout << bbb.ToString() << '\n';

    TileConfig tile_data;
    std::vector<int8_t> src1(MAX), src2(MAX);
    std::vector<int32_t> res(MAX / 4);
    int rows = MAX_ROWS, cols = MAX_COLS;

    if (!set_tiledata_use())
        exit(-1);

    tile_data.init();

    init_buffer<int8_t>(src1, 2);
    print_buffer(src1, rows, cols);

    init_buffer<int8_t>(src2, 3);
    print_buffer(src2, rows, cols);

    init_buffer(res, 0);

    _tile_loadd(2, src1.data(), STRIDE);
    _tile_loadd(3, src2.data(), STRIDE);

    _tile_dpbssd(1, 2, 3);

    _tile_stored(1, res.data(), STRIDE);
    print_buffer<int32_t>(res, rows, cols / 4);

    _tile_release();
}
