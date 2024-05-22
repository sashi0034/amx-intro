#ifndef AMX_INTRO_AMXCALCULATOR_H
#define AMX_INTRO_AMXCALCULATOR_H

#include <memory>
#include "Matrix.h"

namespace amx {
    void InitAmxCalculation();

    void EndAmxCalculation();

    [[nodiscard]]
    Wards16x16 ComputeAmxDotProduct(const Bytes16x64 &a, const Bytes16x64 &b);
} // amx

#endif //AMX_INTRO_AMXCALCULATOR_H
