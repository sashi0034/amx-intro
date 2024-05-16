#ifndef AMX_INTRO_AMXCALCULATOR_H
#define AMX_INTRO_AMXCALCULATOR_H

#include <memory>
#include "Matrix.h"

namespace amx {

    class AmxCalculator {
    public:
        AmxCalculator();

        Wards16x16 DotProduct(const Bytes16x64 &a, const Bytes16x64 &b) const;

    private:
        struct Impl;

        std::shared_ptr<Impl> p_impl;
    };

} // amx

#endif //AMX_INTRO_AMXCALCULATOR_H
