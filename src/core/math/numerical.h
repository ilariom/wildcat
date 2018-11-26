#ifndef WKT_NUMERICAL_H
#define WKT_NUMERICAL_H

#include "math/wktmath.h"
#include <math.h>

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

constexpr inline float toDegree(float angleinRadians)
{
    return 57.2958f * angleinRadians;
}

constexpr inline float toRadians(float angleInDegrees)
{
    return 0.0174533f * angleInDegrees;
}

}}

#endif