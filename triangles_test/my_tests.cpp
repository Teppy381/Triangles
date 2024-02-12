#include <gtest/gtest.h>
#include "../geometry.hpp"

TEST(Triangles, Simple_Intersection1)
{
    geometry::Boxed_triangle_t triangle1{{-3, 3, 0}, {3, 3, 0}, {-3, -3, 0}};
    geometry::Boxed_triangle_t triangle2{{0, 0, 0}, {3, 0, 0}, {3, -3, 0}};

    ASSERT_EQ(true, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection2)
{
    geometry::Boxed_triangle_t triangle1{{-1, 0, 0}, {1, 1, 0}, {1, -1, 0}};
    geometry::Boxed_triangle_t triangle2{{-3, 0, 0}, {3, 3, 0}, {3, -3, 0}};

    ASSERT_EQ(true, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection3)
{
    geometry::Boxed_triangle_t triangle1{{-3, -3, 0}, {-3, 3, 0}, {3, 3, 0}};
    geometry::Boxed_triangle_t triangle2{{-1, -1, 0}, {-1, 3, 0}, {1, 3, 0}};

    ASSERT_EQ(true, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection4)
{
    geometry::Boxed_triangle_t triangle1{{-3, 0, 0}, {-3, 3, 0}, {3, 0, 0}};
    geometry::Boxed_triangle_t triangle2{{-2, 0, 0}, {-2, -3, 0}, {2, 0, 0}};

    ASSERT_EQ(true, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection5)
{
    geometry::Boxed_triangle_t triangle1{{-3, 3, 0}, {3, 3, 0}, {-3, -3, 0}};
    geometry::Boxed_triangle_t triangle2{{-3, 3, 0}, {3, 3, 0}, {-3, 0, 0}};

    ASSERT_EQ(true, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection6)
{
    geometry::Boxed_triangle_t triangle1{{-3, 3, 0}, {3, 3, 0}, {-3, -1, 0}};
    geometry::Boxed_triangle_t triangle2{{3, -3, 0}, {-3, -3, 0}, {-3, 1, 0}};

    ASSERT_EQ(true, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection7)
{
    geometry::Boxed_triangle_t triangle1{{-3, 3, 0}, {3, 3, 0}, {-3, -3, 0}};
    geometry::Boxed_triangle_t triangle2{{3, 2, 0}, {-2, -3, 0}, {-2, 2, 0}};

    ASSERT_EQ(true, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection8)
{
    geometry::Boxed_triangle_t triangle1{{-3, 3, 0}, {3, 3, 0}, {-3, -3, 0}};
    geometry::Boxed_triangle_t triangle2{{3, 0, 0}, {3, -3, 0}, {-2, 2, 0}};

    ASSERT_EQ(true, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection9)
{
    geometry::Boxed_triangle_t triangle1{{-1, 1, 0}, {3, 1, 0}, {-1, -3, 0}};
    geometry::Boxed_triangle_t triangle2{{0, 0, 0}, {-3, 0, 0}, {0, 3, 0}};

    ASSERT_EQ(true, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection10)
{
    geometry::Boxed_triangle_t triangle1{{-1, 1, 0}, {3, 1, 0}, {-1, -3, 0}};
    geometry::Boxed_triangle_t triangle2{{0, 2, 0}, {3, -3, 0}, {0, -3, 0}};

    ASSERT_EQ(true, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection11)
{
    geometry::Boxed_triangle_t triangle1{{-2, 2, 0}, {3, 2, 0}, {-2, -3, 0}};
    geometry::Boxed_triangle_t triangle2{{-3, 2, 0}, {0, 3, 0}, {0, -3, 0}};

    ASSERT_EQ(true, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection12)
{
    geometry::Boxed_triangle_t triangle1{{0, 3, 0}, {-3, 0, 0}, {-3, 3, 0}};
    geometry::Boxed_triangle_t triangle2{{3, 3, 0}, {0, 3, 0}, {3, 0, 0}};

    ASSERT_EQ(true, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection13)
{
    geometry::Boxed_triangle_t triangle1{{0, 3, 0}, {-3, 0, 0}, {-3, 3, 0}};
    geometry::Boxed_triangle_t triangle2{{3, 0, 0}, {3, -3, 0}, {0, -3, 0}};

    ASSERT_EQ(false, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection14)
{
    geometry::Boxed_triangle_t triangle1{{-2, 0, 0}, {2, 0, 0}, {0, 0, 2}};
    geometry::Boxed_triangle_t triangle2{{0, 0, 1}, {2, 2, 0}, {-2, -2, 0}};

    ASSERT_EQ(true, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection15)
{
    geometry::Boxed_triangle_t triangle1{{0, -1.5, 0}, {0, -1, 0}, {2, 3, 2}};
    geometry::Boxed_triangle_t triangle2{{-1.55, 1.34, 0}, {-1.83, -1.42, 0}, {-2.54, 1.3, 2}};

    ASSERT_EQ(false, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection16)
{
    geometry::Boxed_triangle_t triangle1{{0, -1.5, 0}, {0, -1, 0}, {0, 0, 2}};
    geometry::Boxed_triangle_t triangle2{{-1, 1.78, 0}, {-1, -2.38, 0}, {-1, 0, 3}};

    ASSERT_EQ(false, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection17)
{
    geometry::Boxed_triangle_t triangle1{{201, -753, 646}, {-65, -910, -312}, {103, -13, -963}};
    geometry::Boxed_triangle_t triangle2{{-85, -277, 871}, {670, -132, -661}, {-1000, 343, 585}};

    ASSERT_EQ(false, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection18)
{
    geometry::Boxed_triangle_t triangle1{{201, -753, 646}, {-65, -910, -312}, {103, -13, -963}};
    geometry::Boxed_triangle_t triangle2{{-736, 370, -74}, {-538, -307, -478}, {341, -773, 903}};

    ASSERT_EQ(false, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection19)
{
    geometry::Boxed_triangle_t triangle1{{201, -753, 646}, {-65, -910, -312}, {103, -13, -963}};
    geometry::Boxed_triangle_t triangle2{{644, 181, 138 }, {269, 679, 388}, {784, 669, 353}};

    ASSERT_EQ(false, geometry::lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Simple_Intersection20)
{
    geometry::Boxed_triangle_t triangle1{{-3, 3, 0}, {3, 3, 0}, {-3, -3, 0}};
    geometry::Boxed_triangle_t triangle2{{0, 0, 0}, {-3, 0, 0}, {0, 3, 0}};

    ASSERT_EQ(true, geometry::lookup_intersection(triangle1, triangle2));
}

// TEST(Triangles, E2E_1)
// {
// }



