#ifndef WKT_NUMERICAL_H
#define WKT_NUMERICAL_H

#include "core/math/math.h"
#include <cmath>

namespace wkt {
namespace math
{

inline float clamp(float val, float m, float M)
{
    return std::max(m, std::min(val, M));
}

inline float clampNorm(float val)
{
    return clamp(val, .0f, 1.f);
}

}}

#endif