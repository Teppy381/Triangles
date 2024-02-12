#pragma once

#include <cmath>

static const float tolerance = 0.00001;

namespace geometry
{

bool is_zero(float a)
{
    if (std::abs(a) < tolerance)
    {
        return true;
    }
    return false;
}

bool equal(float a, float b)
{
    return is_zero(a - b);
}

float min(float x1, float x2, float x3)
{
    float min = x1;
    if (x2 < min) {min = x2;}
    if (x3 < min) {min = x3;}
    return min;
}

float max(float x1, float x2, float x3)
{
    float max = x1;
    if (x2 > max) {max = x2;}
    if (x3 > max) {max = x3;}
    return max;
}

} // namespace geometry
