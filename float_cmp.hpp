#include <cmath>

static const float tolerance = 0.00001;

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

