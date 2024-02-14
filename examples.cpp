#include <iostream>
#include "input.hpp"
#include <vector>
#include <memory>

#define SHOW(...) \
    std::cout << #__VA_ARGS__ << " == " << __VA_ARGS__ << '\n'

int point_and_vector();
int products();
int vectors();
int point_and_triangle();
int line_and_line();
int triangle_and_triangle();

int main()
{
    int calls = 0;

    size_t count;
    std::cin >> count;
    std::unique_ptr<bool[]> intersection_count{new bool[count]()};

    std::vector<geometry::Boxed_triangle_t> triangles;

    for(size_t i = 0; i != count; ++i)
    {
        triangles.push_back(geometry::scan_boxed_triangle());
    }
    for(size_t i = 0; i != count; ++i)
    {
        if (intersection_count[i] == true)
        {
            continue;
        }
        for(size_t j = 0; j < i; ++j)
        {
            if (intersection_count[j] == false)
            {
                continue;
            }
            calls++;
            if (geometry::lookup_intersection(triangles[i], triangles[j]))
            {
                intersection_count[i] = true;
                // intersection_count[j] already true;
                break;
            }
        }
        for(size_t j = i + 1; j < count; ++j)
        {
            calls++;
            if (geometry::lookup_intersection(triangles[i], triangles[j]))
            {
                intersection_count[i] = true;
                intersection_count[j] = true;
                break;
            }
        }
    }

    bool found_intersection = false;
    for(size_t i = 0; i != count; ++i)
    {
        if (intersection_count[i])
        {
            std::cout << i << " ";
            found_intersection = true;
        }
    }
    if (!found_intersection)
    {
        std::cout << "No intersections found";

    }
    std::cout << "calls: " << calls << std::endl;
    return 0;
}

int triangle_and_triangle()
{
    geometry::Boxed_triangle_t triangle1{{-1, 0, 0}, {1, 1, 0}, {1, -1, 0}};
    geometry::Boxed_triangle_t triangle2{{-3, 0, 0}, {3, 3, 0}, {3, -3, 0}};

    if (lookup_intersection(triangle1, triangle2))
    {
        std::cout << "Intersection found!!" << std::endl;
        return 0;
    }
    std::cout << "No intersection!!" << std::endl;
    return 0;
}



int line_and_line()
{
    geometry::Point_t A1{2, 0, 0};
    geometry::Point_t A2{0, 2, 0};
    geometry::Point_t B1{0, 0, 0};

    float x1, x2, x3;
    std::cin >> x1 >> x2 >> x3;
    geometry::Point_t B2{x1, x2, x3};

    std::cout << line_segment_line_segment_coplanar_intersection(A1, A2, B1, B2) << std::endl;
    return 0;
}

int point_and_triangle()
{
    geometry::Triangle_t my_triangle{{0, 0, 0}, {2, 0, 0}, {0, 2, 0}};
    float x1, x2, x3;
    std::cin >> x1 >> x2 >> x3;

    geometry::Point_t my_point{x1, x2, x3};

    SHOW(triangle_point_coplanar_intersection(my_triangle, my_point));
    return 0;
}


int vectors()
{
    float x1, x2, x3;
    std::cin >> x1 >> x2 >> x3;
    std::cout << "max: " << geometry::max(x1, x2, x3) << std::endl;
    std::cout << "min: " << geometry::min(x1, x2, x3) << std::endl;

    geometry::Vector_t vec{x1, x2, x3};
    (vec * 100).dump();
    (-vec).dump();
    (vec + vec).dump();
    vec.dump();
    return 0;
}

int products()
{
    geometry::Vector_t v1{1, 1, 0};
    geometry::Vector_t v2{0, 1, 1};
    geometry::Vector_t v3{1, 0, 1};
    SHOW(dot_product(v1, v2));

    std::cout << "cross_product(v1, v2) == " ;
    cross_product(v1, v2).dump();

    SHOW(triple_product(v1, v2, v3));
    return 0;
}


int point_and_vector()
{
    std::cout << "HELLO!!!" << std::endl;

    geometry::Point_t my_point{23, 252, 235};

    geometry::Vector_t my_vector{8, 6, 0};
    geometry::Vector_t second_vector = {3, 4, 4};
    second_vector = {23, 252, 235};

    my_point.dump();
    my_vector.normalize().dump();
    second_vector.dump();

    SHOW((my_point == second_vector));

    return 0;
}
