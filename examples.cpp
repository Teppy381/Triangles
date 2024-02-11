#include <iostream>
#include "geometry.hpp"

#define SHOW(...) \
    std::cout << #__VA_ARGS__ << " == " << __VA_ARGS__ << '\n'

int point_and_vector();
int products();
int vectors();
int point_and_triangle();
int line_and_line();

int main()
{
    Boxed_triangle_t triangle1{{2, 0, 0}, {0, 2, 0}, {0, 0, 0}};

    float x1, y1, z1,
          x2, y2, z2,
          x3, y3, z3;
    std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;

    Boxed_triangle_t triangle2{{x1, y1, z1}, {x2, y2, z2}, {x3, y3, z3}};

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
    Point_t A1{2, 0, 0};
    Point_t A2{0, 2, 0};
    Point_t B1{0, 0, 0};

    float x1, x2, x3;
    std::cin >> x1 >> x2 >> x3;
    Point_t B2{x1, x2, x3};

    std::cout << line_segment_line_segment_coplanar_intersection(A1, A2, B1, B2) << std::endl;
    return 0;
}

int point_and_triangle()
{
    Triangle_t my_triangle{{0, 0, 0}, {2, 0, 0}, {0, 2, 0}};
    float x1, x2, x3;
    std::cin >> x1 >> x2 >> x3;

    Point_t my_point{x1, x2, x3};

    SHOW(triangle_point_coplanar_intersection(my_triangle, my_point));
    return 0;
}


int vectors()
{
    float x1, x2, x3;
    std::cin >> x1 >> x2 >> x3;
    std::cout << "max: " << max(x1, x2, x3) << std::endl;
    std::cout << "min: " << min(x1, x2, x3) << std::endl;

    Vector_t vec{x1, x2, x3};
    (vec * 100).dump();
    (-vec).dump();
    (vec + vec).dump();
    vec.dump();
    return 0;
}

int products()
{
    Vector_t v1{1, 1, 0};
    Vector_t v2{0, 1, 1};
    Vector_t v3{1, 0, 1};
    SHOW(dot_product(v1, v2));

    std::cout << "cross_product(v1, v2) == " ;
    cross_product(v1, v2).dump();

    SHOW(triple_product(v1, v2, v3));
    return 0;
}


int point_and_vector()
{
    std::cout << "HELLO!!!" << std::endl;

    Point_t my_point{23, 252, 235};

    Vector_t my_vector{8, 6, 0};
    Vector_t second_vector = {3, 4, 4};
    second_vector = {23, 252, 235};

    my_point.dump();
    my_vector.normalize().dump();
    second_vector.dump();

    SHOW((my_point == second_vector));

    return 0;
}
