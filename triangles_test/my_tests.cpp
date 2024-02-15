#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include "../input.hpp"

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


const std::string filepath = "..\\..\\triangles_test\\E2E tests\\";

void test_file(std::string filename)
{
    std::ifstream input{filepath + filename};
    if (!input.is_open())
    {
        throw std::runtime_error{"Unable to open file " + filepath + filename};
    }
    auto cinbuf = std::cin.rdbuf(input.rdbuf());

    std::vector<geometry::Boxed_triangle_t> triangles = geometry::scan_triangles();
    std::vector<bool> intersection_list = geometry::check_for_intersections(triangles);

    std::vector<bool> correct_intersection_list;
    for(int i = 0; i < triangles.size(); ++i)
    {
        correct_intersection_list.emplace_back(false);
    }

    size_t num;
    std::cin >> num;
    while (std::cin.good())
    {
        correct_intersection_list[num] = true;
        std::cin >> num;
    }

    std::cin.rdbuf(cinbuf);

    ASSERT_EQ(intersection_list, correct_intersection_list);
}

TEST(Triangles, E2E_1)
{
    test_file("1.txt");
}

TEST(Triangles, E2E_2)
{
    test_file("2.txt");
}

TEST(Triangles, E2E_3)
{
    test_file("3.txt");
}

TEST(Triangles, E2E_4)
{
    test_file("4.txt");
}

TEST(Triangles, E2E_5)
{
    test_file("5.txt");
}

TEST(Triangles, E2E_6)
{
    test_file("6.txt");
}

TEST(Triangles, E2E_7)
{
    test_file("7.txt");
}

TEST(Triangles, E2E_8)
{
    test_file("8.txt");
}

// unsure, python script cannot solve
TEST(Triangles, E2E_9)
{
    test_file("9.txt");
}

// unsure, python script cannot solve
TEST(Triangles, E2E_10)
{
    test_file("10.txt");
}
