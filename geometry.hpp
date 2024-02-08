#include "float_cmp.hpp"
#include "cassert"
#include <iostream>

struct Point_t
{
    float x = NAN, y = NAN, z = NAN;

    //constructor
    Point_t(float x_, float y_, float z_): x(x_), y(y_), z(z_)
    {}

    Point_t() = default;

    bool is_valid() const
    {
        return !(std::isnan(x) || std::isnan(y) || std::isnan(z));
    }

    bool operator==(const Point_t &rhs) const
    {
        assert((*this).is_valid() && rhs.is_valid());
        return (equal(x, rhs.x) && equal(y, rhs.y) && equal(z, rhs.z));
    }

    void dump() const
    {
        assert((*this).is_valid());
        std::cout << "(" << x << ", " << y << ", " << z << ")\n";
    }
};

struct Vector_t : public Point_t
{
    using Point_t::Point_t;

    Vector_t normalize()
    {
        float length = std::sqrt(x*x + y*y + z*z);
        x /= length;
        y /= length;
        z /= length;
        return *this;
    }

};

Vector_t cross_product(const Vector_t& vec1, const Vector_t& vec2)
{
    assert(vec1.is_valid() && vec2.is_valid());

    Vector_t result = {vec1.y * vec2.z - vec1.z * vec2.y,
                       vec1.z * vec2.x - vec1.x * vec2.z,
                       vec1.x * vec2.y - vec1.y * vec2.x};
    return result;
}

float dot_product(const Vector_t& vec1, const Vector_t& vec2)
{
    assert(vec1.is_valid() && vec2.is_valid());
    float result = vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
    return result;
}