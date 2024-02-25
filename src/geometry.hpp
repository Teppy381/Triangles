#pragma once

#include "float_cmp.hpp"
#include "cassert"
#include <iostream>
#include <cmath>

namespace geometry
{

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

    bool operator==(const Point_t& rhs) const
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

    // vector from {0, 0, 0} to P
    Vector_t(Point_t P) : Point_t(P.x, P.y, P.z)
    {}

    // vector from P1 to P2
    Vector_t(Point_t P1, Point_t P2) : Point_t{P2.x - P1.x, P2.y - P1.y, P2.z - P1.z}
    {}

    float length() const
    {
        return std::sqrt(x*x + y*y + z*z);
    }

    Vector_t normalize() const
    {
        float length = (*this).length();
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

    Vector_t operator*(float rhs) const
    {
        return Vector_t{x * rhs, y * rhs, z * rhs};
    }

};

static bool is_zero(const Vector_t& vec)
{
    return is_zero(vec.x) && is_zero(vec.y) && is_zero(vec.z);
}

static Vector_t cross_product(const Vector_t& vec1, const Vector_t& vec2)
{
    assert(vec1.is_valid() && vec2.is_valid());

    Vector_t result = {vec1.y * vec2.z - vec1.z * vec2.y,
                       vec1.z * vec2.x - vec1.x * vec2.z,
                       vec1.x * vec2.y - vec1.y * vec2.x};
    return result;
}

static float dot_product(const Vector_t& vec1, const Vector_t& vec2)
{
    assert(vec1.is_valid() && vec2.is_valid());
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

static float triple_product(const Vector_t& vec1, const Vector_t& vec2, const Vector_t& vec3)
{
    return dot_product(vec1, cross_product(vec2, vec3));
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

    void dump() const
    {
        std::cout << "Triangle:" << std::endl;

        P.dump();
        (e1 + P).dump();
        (e2 + P).dump();
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


static bool boxes_intersect(const Boxed_triangle_t& tr1, const Boxed_triangle_t& tr2)
{
    if ((tr2.P_min.x > tr1.P_max.x) || (tr2.P_min.y > tr1.P_max.y) || (tr2.P_min.z > tr1.P_max.z)
     || (tr1.P_min.x > tr2.P_max.x) || (tr1.P_min.y > tr2.P_max.y) || (tr1.P_min.z > tr2.P_max.z))
    {
        return false; //no intersection
    }
    return true;
}

static bool triangle_point_coplanar_intersection(const Triangle_t& tr, const Point_t& A)
{
    Vector_t vec{tr.P, A};
    // decomposition: vec == tr.e1 * t1 + tr.e2 * t2
    float x1 = tr.e1.x,
          y1 = tr.e1.y,
          z1 = tr.e1.z,
          x2 = tr.e2.x,
          y2 = tr.e2.y,
          z2 = tr.e2.z,
          xA = vec.x,
          yA = vec.y,
          zA = vec.z;

    float t2 = (x1*zA - xA*z1)/(x1*z2 - x2*z1);
    if (!std::isfinite(t2))
    {
        t2 = (x1*yA - xA*y1)/(x1*y2 - x2*y1);
        if (!std::isfinite(t2))
        {
            std::cout << "Math error!" << std::endl;
            exit(1);
        }
    }
    float t1 = (xA - x2*t2)/x1;
    if (!std::isfinite(t1))
    {
        t1 = (yA - y2*t2)/y1;
        if (!std::isfinite(t1))
        {
            t1 = (zA - z2*t2)/z1;
            if (!std::isfinite(t1))
            {
                std::cout << "Math error!" << std::endl;
                exit(1);
            }
        }
    }
    if (t1 < 0 || t1 > 1 || t2 < 0 || t2 > 1 || (t1 + t2) < 0 || (t1 + t2) > 1)
    {
        return false;
    }
    return true;
}

// assuming that all points are on the same line
static bool point_in_line_segment_intersection(const Point_t& A1, const Point_t& A2, const Point_t& B)
{
    if (!equal(A1.x, A2.x))
    {
        return (B.x <= A1.x && B.x >= A2.x) || (B.x >= A1.x && B.x <= A2.x);
    }
    else if (!equal(A1.y, A2.y))
    {
        return (B.y <= A1.y && B.y >= A2.y) || (B.y >= A1.y && B.y <= A2.y);
    }
    else if (!equal(A1.z, A2.z))
    {
        return (B.z <= A1.z && B.z >= A2.z) || (B.z >= A1.z && B.z <= A2.z);
    }
    else // A1 == A2
    {
        // throw std::runtime_error("point_in_line_segment_intersection: points in line segment are equal!");
        return A1 == B;
    }
}

static bool line_segment_line_segment_coplanar_intersection(const Point_t& A1, const Point_t& A2,
                                                     const Point_t& B1, const Point_t& B2)
{
    Vector_t normal = cross_product({A1, A2}, {B1, B2});
    if (is_zero(normal)) // => collinear lines
    {
        return is_zero(cross_product({A1, A2}, {A1, B2}));
    }

    float sig1 = triple_product({A1, A2}, normal, {A1, B2});
    if (is_zero(sig1))
    {
        return point_in_line_segment_intersection(A1, A2, B2);
    }
    float sig2 = triple_product({A1, A2}, normal, {A1, B1});
    if (is_zero(sig1))
    {
        return point_in_line_segment_intersection(A1, A2, B1);
    }
    if (sig1 * sig2 > 0)
    {
        return false;
    }

    float sig3 = triple_product({B1, B2}, normal, {B1, A2});
    if (is_zero(sig1))
    {
        return point_in_line_segment_intersection(B1, B2, A2);
    }
    float sig4 = triple_product({B1, B2}, normal, {B1, A1});
    if (is_zero(sig1))
    {
        return point_in_line_segment_intersection(B1, B2, A1);
    }
    if (sig3 * sig4 > 0)
    {
        return false;
    }

    return true;
}


static bool triangle_line_segment_coplanar_intersection(const Triangle_t& tr, const Point_t& P1, const Point_t& P2)
{
    Point_t T1 = tr.P;
    Point_t T2 = tr.e1 + tr.P;
    Point_t T3 = tr.e2 + tr.P;

    return line_segment_line_segment_coplanar_intersection(P1, P2, T1, T2) ||
           line_segment_line_segment_coplanar_intersection(P1, P2, T1, T3) ||
           line_segment_line_segment_coplanar_intersection(P1, P2, T2, T3);
}

static bool lookup_coplanar_intersection(const Triangle_t& tr1, const Triangle_t& tr2)
{
    Point_t T11 = tr1.P;
    Point_t T12 = tr1.e1 + tr1.P;
    Point_t T13 = tr1.e2 + tr1.P;
    Point_t T21 = tr2.P;
    Point_t T22 = tr2.e1 + tr2.P;
    Point_t T23 = tr2.e2 + tr2.P;
    return triangle_line_segment_coplanar_intersection(tr2, T11, T12) ||
           triangle_line_segment_coplanar_intersection(tr2, T11, T13) ||
           triangle_line_segment_coplanar_intersection(tr2, T12, T13) ||
           triangle_line_segment_coplanar_intersection(tr1, T21, T22) ||
           triangle_line_segment_coplanar_intersection(tr1, T21, T23) ||
           triangle_line_segment_coplanar_intersection(tr1, T22, T23) ||
           triangle_point_coplanar_intersection(tr1, T21) ||
           triangle_point_coplanar_intersection(tr2, T11);
}

static bool triangle_line_segment_intersection(const Triangle_t& tr, const Point_t P1, const Point_t P2)
{
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

    Vector_t U{P1, P2};

    float dot = dot_product(normal, U);
    if (is_zero(dot)) {return false;} // parallel to triangle

    Vector_t W{tr.P, P1};

    float fac = -dot_product(normal, W) / dot;
    if (fac < 0 || fac > 1)
    {
        return false;
    }
    Point_t point = Vector_t{P1} + (U * fac);

    return triangle_point_coplanar_intersection(tr, point);
}


static bool lookup_non_coplanar_intersection(const Triangle_t& tr1, const Triangle_t& tr2)
{
    return
    triangle_line_segment_intersection(tr1, tr2.P, (Vector_t) tr2.P + tr2.e1) ||
    triangle_line_segment_intersection(tr1, tr2.P, (Vector_t) tr2.P + tr2.e2) ||
    triangle_line_segment_intersection(tr1, (Vector_t) tr2.P + tr2.e1, (Vector_t) tr2.P + tr2.e2) ||
    triangle_line_segment_intersection(tr2, tr1.P, (Vector_t) tr1.P + tr1.e1) ||
    triangle_line_segment_intersection(tr2, tr1.P, (Vector_t) tr1.P + tr1.e2) ||
    triangle_line_segment_intersection(tr2, (Vector_t) tr1.P + tr1.e1, (Vector_t) tr1.P + tr1.e2);
}

static bool lookup_intersection(const Boxed_triangle_t& tr1, const Boxed_triangle_t& tr2)
{
    assert(tr1.valid() && tr2.valid());

    if (boxes_intersect(tr1, tr2) == false)
    {
        // std::cout << "Boxes do not intersect" << std::endl;
        return false;
    }

    Vector_t R = Vector_t{tr1.P, tr2.P}; // vector from tr1.P to tr2.P

    Vector_t n1 = tr1.get_normal();
    if ((!n1.is_valid()) || is_zero(n1))
    {
        std::cout << "Invalid triangle, cannot calculate normal" << std::endl;
        tr1.dump();
        exit(2);
    }
    Vector_t n2 = tr2.get_normal();
    if ((!n2.is_valid()) || is_zero(n2))
    {
        std::cout << "Invalid triangle, cannot calculate normal" << std::endl;
        tr2.dump();
        exit(2);
    }

    if (n1 == n2 || n1 == -n2)
    {
        if (is_zero(dot_product(n1, R))) // triangles are in the same plane
        {
            // std::cout << "Triangles are in the same plane" << std::endl;
            return lookup_coplanar_intersection(tr1, tr2);
        }
        return false; // parallel planes
    }
    return lookup_non_coplanar_intersection(tr1, tr2);
}

} // namespace geometry
