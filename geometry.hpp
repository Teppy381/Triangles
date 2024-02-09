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

    // vector from P1 to P2
    Vector_t(Point_t P1, Point_t P2) : Point_t{P2.x - P1.x, P2.y - P1.y, P2.z - P1.z}
    {}

    Vector_t normalize() const
    {
        float length = std::sqrt(x*x + y*y + z*z);
        return Vector_t{x/length, y/length, z/length};
    }

    Vector_t operator-() const
    {
        return Vector_t{-x, -y, -z};
    }

    Vector_t operator+(const Vector_t& rhs) const
    {
        return Vector_t{x + rhs.x, y + rhs.y, z + rhs.z};
    }

    Vector_t operator-(const Vector_t& rhs) const
    {
        return Vector_t{x - rhs.x, y - rhs.y, z - rhs.z};
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
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

float triple_product(const Vector_t& vec1, const Vector_t& vec2, const Vector_t& vec3)
{
    return dot_product(vec1, cross_product(vec2, vec3));
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


struct Triangle_t
{
    Point_t P;
    Vector_t e1, e2; // vectors from P to two other points

    Triangle_t(const Point_t& P1, const Point_t& P2, const Point_t& P3) : P{P1}
    {
        e1 = {P2.x - P1.x, P2.y - P1.y, P2.z - P1.z};
        e2 = {P3.x - P1.x, P3.y - P1.y, P3.z - P1.z};
    }

    Vector_t get_normal() const
    {
        return cross_product(e1, e2).normalize();
    }

    bool valid() const
    {
        return (P.is_valid() && e1.is_valid() && e2.is_valid());
    }
};

struct Boxed_triangle_t : public Triangle_t
{
    Point_t P_min, P_max; // opposite corners of the box containing triangle

    Boxed_triangle_t(const Point_t& P1, const Point_t& P2, const Point_t& P3) : Triangle_t(P1, P2, P3)
    {
        P_min.x = min(P1.x, P2.x, P3.x);
        P_min.y = min(P1.y, P2.y, P3.y);
        P_min.z = min(P1.z, P2.z, P3.z);

        P_max.x = max(P1.x, P2.x, P3.x);
        P_max.y = max(P1.y, P2.y, P3.y);
        P_max.z = max(P1.z, P2.z, P3.z);

        assert(P_min.is_valid() && P_max.is_valid());
    }
};


bool boxes_intersect(const Boxed_triangle_t& tr1, const Boxed_triangle_t& tr2)
{
    if ((tr2.P_min.x > tr1.P_max.x) || (tr2.P_min.y > tr1.P_max.y) || (tr2.P_min.z > tr1.P_max.z)
     || (tr1.P_min.x > tr2.P_max.x) || (tr1.P_min.y > tr2.P_max.y) || (tr1.P_min.z > tr2.P_max.z))
    {
        return false; //no intersection
    }
    return true;
}

bool triangle_point_coplanar_intersection(Triangle_t& tr, Point_t& A)
{
    return false; // TODO
}

bool triangle_line_segment_coplanar_intersection(Triangle_t& tr, Point_t& P1, Point_t& P2)
{
    return false; // TODO
}

bool lookup_coplanar_intersection(Triangle_t& tr1, Triangle_t& tr2)
{
    return false; // TODO
}

bool triangle_line_segment_intersection(Triangle_t& tr, Point_t& P1, Point_t& P2)
{
    Vector_t seg{P1, P2};
    Vector_t L1{P1, tr.P};
    Vector_t L2{P2, tr.P};
    Vector_t normal = cross_product(tr.e1, tr.e2);

    // different signs in L1*normal and L2*normal => P1 and P2 are in different half-spaces
    float dot1 = dot_product(L1, normal);
    float dot2 = dot_product(L2, normal);

    // one or both points might be in the triangle's plane
    if (is_zero(dot1) && is_zero(dot2))
    {
        return triangle_line_segment_coplanar_intersection(tr, P1, P2);
    }
    else if (is_zero(dot1)) // => we need to check if P1 is inside the triangle
    {
        return triangle_point_coplanar_intersection(tr, P1);
    }
    else if (is_zero(dot2)) // => we need to check if P2 is inside the triangle
    {
        return triangle_point_coplanar_intersection(tr, P2);
    }
    else if (dot1 * dot2 > 0)
    {
        return false;
    }
    // else (P1 and P2 are in different half-spaces)
    return false; // TODO
}

bool lookup_non_coplanar_intersection(Triangle_t& tr1, Triangle_t& tr2)
{
    return false; // TODO
}

bool lookup_intersection(Boxed_triangle_t& tr1, Boxed_triangle_t& tr2)
{
    assert(tr1.valid() && tr2.valid());

    if (boxes_intersect(tr1, tr2) == false)
    {
        return false;
    }

    Vector_t R = Vector_t{tr1.P, tr2.P}; // vector from tr1.P to tr2.P

    Vector_t n1 = tr1.get_normal();
    Vector_t n2 = tr2.get_normal();

    if (n1 == n2 || n1 == -n2)
    {
        if (is_zero(dot_product(n1, R))) // triangles are in the same plane
        {
            return lookup_coplanar_intersection(tr1, tr2);
        }
        return false; // parallel planes
    }
    return lookup_non_coplanar_intersection(tr1, tr2);
}