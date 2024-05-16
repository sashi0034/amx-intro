#include <iostream>
#include <vector>
#include <array>
#include <unistd.h>
#include <sys/syscall.h>
#include <cstdint>
#include "AmxCalculator.h"

namespace {
    constexpr int ARCH_GET_XCOMP_PERM = 0x1022;
    constexpr int ARCH_REQ_XCOMP_PERM = 0x1023;
    constexpr int XFEATURE_XTILECFG = 17;
    constexpr int XFEATURE_XTILEDATA = 18;

    struct TileConfig {
        uint8_t paletteId = 0;
        uint8_t startRow = 0;
        uint8_t reserved_0[14] = {0};
        std::array<uint16_t, 16> cols = {0};
        std::array<uint8_t, 16> rows = {0};
    };

    bool setTiledataUse() {
        if (syscall(SYS_arch_prctl, ARCH_REQ_XCOMP_PERM, XFEATURE_XTILEDATA)) {
            std::cerr << "\nFail to set XFEATURE_XTILEDATA\n\n";
            throw std::runtime_error("Fail to set XFEATURE_XTILEDATA");
        }

        std::cout << "\nTILE DATA USE SET - OK\n\n";
    }

    using namespace amx;
}

struct AmxCalculator::Impl {
    TileConfig config;

    Impl() {
        setTiledataUse();

        config.paletteId = 1;
        config.startRow = 0;

        // TODO: 変更を可能にする
        config.cols[0] = 64;
        config.rows[0] = 16;

        config.cols[1] = 64;
        config.rows[1] = 16;

        config.cols[2] = 64;
        config.rows[2] = 16;

        _tile_loadconfig(this);
    }

    ~Impl() {
        _tile_release();
    }
};

namespace amx {

    AmxCalculator::AmxCalculator() :
            p_impl(std::make_shared<Impl>()) {
    }

    Wards16x16 AmxCalculator::DotProduct(const Bytes16x64 &a, const Bytes16x64 &b) const {
        constexpr int stride = 64;

        _tile_loadd(1, a.bytes.data(), stride);
        _tile_loadd(2, a.bytes.data(), stride);

        _tile_dpbssd(0, 1, 2);

        Wards16x16 result;
        _tile_stored(0, result.bytes.data(), stride);

        return result;
    }
} // amx