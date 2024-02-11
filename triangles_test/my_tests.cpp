#include <gtest/gtest.h>
#include "../geometry.hpp"

TEST(Triangles, Intersection_test1)
{
    Boxed_triangle_t triangle1{{-3, 3, 0}, {3, 3, 0}, {-3, -3, 0}};
    Boxed_triangle_t triangle2{{0, 0, 0}, {3, 0, 0}, {3, -3, 0}};

    ASSERT_EQ(true, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test2)
{
    Boxed_triangle_t triangle1{{-1, 0, 0}, {1, 1, 0}, {1, -1, 0}};
    Boxed_triangle_t triangle2{{-3, 0, 0}, {3, 3, 0}, {3, -3, 0}};

    ASSERT_EQ(true, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test3)
{
    Boxed_triangle_t triangle1{{-3, -3, 0}, {-3, 3, 0}, {3, 3, 0}};
    Boxed_triangle_t triangle2{{-1, -1, 0}, {-1, 3, 0}, {1, 3, 0}};

    ASSERT_EQ(true, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test4)
{
    Boxed_triangle_t triangle1{{-3, 0, 0}, {-3, 3, 0}, {3, 0, 0}};
    Boxed_triangle_t triangle2{{-2, 0, 0}, {-2, -3, 0}, {2, 0, 0}};

    ASSERT_EQ(true, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test5)
{
    Boxed_triangle_t triangle1{{-3, 3, 0}, {3, 3, 0}, {-3, -3, 0}};
    Boxed_triangle_t triangle2{{-3, 3, 0}, {3, 3, 0}, {-3, 0, 0}};

    ASSERT_EQ(true, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test6)
{
    Boxed_triangle_t triangle1{{-3, 3, 0}, {3, 3, 0}, {-3, -1, 0}};
    Boxed_triangle_t triangle2{{3, -3, 0}, {-3, -3, 0}, {-3, 1, 0}};

    ASSERT_EQ(true, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test7)
{
    Boxed_triangle_t triangle1{{-3, 3, 0}, {3, 3, 0}, {-3, -3, 0}};
    Boxed_triangle_t triangle2{{3, 2, 0}, {-2, -3, 0}, {-2, 2, 0}};

    ASSERT_EQ(true, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test8)
{
    Boxed_triangle_t triangle1{{-3, 3, 0}, {3, 3, 0}, {-3, -3, 0}};
    Boxed_triangle_t triangle2{{3, 0, 0}, {3, -3, 0}, {-2, 2, 0}};

    ASSERT_EQ(true, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test9)
{
    Boxed_triangle_t triangle1{{-1, 1, 0}, {3, 1, 0}, {-1, -3, 0}};
    Boxed_triangle_t triangle2{{0, 0, 0}, {-3, 0, 0}, {0, 3, 0}};

    ASSERT_EQ(true, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test10)
{
    Boxed_triangle_t triangle1{{-1, 1, 0}, {3, 1, 0}, {-1, -3, 0}};
    Boxed_triangle_t triangle2{{0, 2, 0}, {3, -3, 0}, {0, -3, 0}};

    ASSERT_EQ(true, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test11)
{
    Boxed_triangle_t triangle1{{-2, 2, 0}, {3, 2, 0}, {-2, -3, 0}};
    Boxed_triangle_t triangle2{{-3, 2, 0}, {0, 3, 0}, {0, -3, 0}};

    ASSERT_EQ(true, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test12)
{
    Boxed_triangle_t triangle1{{0, 3, 0}, {-3, 0, 0}, {-3, 3, 0}};
    Boxed_triangle_t triangle2{{3, 3, 0}, {0, 3, 0}, {3, 0, 0}};

    ASSERT_EQ(true, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test13)
{
    Boxed_triangle_t triangle1{{0, 3, 0}, {-3, 0, 0}, {-3, 3, 0}};
    Boxed_triangle_t triangle2{{3, 0, 0}, {3, -3, 0}, {0, -3, 0}};

    ASSERT_EQ(false, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test14)
{
    Boxed_triangle_t triangle1{{-2, 0, 0}, {2, 0, 0}, {0, 0, 2}};
    Boxed_triangle_t triangle2{{0, 0, 1}, {2, 2, 0}, {-2, -2, 0}};

    ASSERT_EQ(true, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test15)
{
    Boxed_triangle_t triangle1{{0, -1.5, 0}, {0, -1, 0}, {2, 3, 2}};
    Boxed_triangle_t triangle2{{-1.55, 1.34, 0}, {-1.83, -1.42, 0}, {-2.54, 1.3, 2}};

    ASSERT_EQ(false, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test16)
{
    Boxed_triangle_t triangle1{{0, -1.5, 0}, {0, -1, 0}, {0, 0, 2}};
    Boxed_triangle_t triangle2{{-1, 1.78, 0}, {-1, -2.38, 0}, {-1, 0, 3}};

    ASSERT_EQ(false, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test17)
{
    Boxed_triangle_t triangle1{{201, -753, 646}, {-65, -910, -312}, {103, -13, -963}};
    Boxed_triangle_t triangle2{{-85, -277, 871}, {670, -132, -661}, {-1000, 343, 585}};

    ASSERT_EQ(false, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test18)
{
    Boxed_triangle_t triangle1{{201, -753, 646}, {-65, -910, -312}, {103, -13, -963}};
    Boxed_triangle_t triangle2{{-736, 370, -74}, {-538, -307, -478}, {341, -773, 903}};

    ASSERT_EQ(false, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test19)
{
    Boxed_triangle_t triangle1{{201, -753, 646}, {-65, -910, -312}, {103, -13, -963}};
    Boxed_triangle_t triangle2{{644, 181, 138 }, {269, 679, 388}, {784, 669, 353}};

    ASSERT_EQ(false, lookup_intersection(triangle1, triangle2));
}

TEST(Triangles, Intersection_test20)
{
    Boxed_triangle_t triangle1{{-3, 3, 0}, {3, 3, 0}, {-3, -3, 0}};
    Boxed_triangle_t triangle2{{0, 0, 0}, {-3, 0, 0}, {0, 3, 0}};

    ASSERT_EQ(true, lookup_intersection(triangle1, triangle2));
}



