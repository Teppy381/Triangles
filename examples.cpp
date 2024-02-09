#include <iostream>
#include "geometry.hpp"

#define SHOW(...) \
    std::cout << #__VA_ARGS__ << " == " << __VA_ARGS__ << '\n'

int point_and_vector();
int products();


int main()
{
    float x1, x2, x3;
    std::cin >> x1 >> x2 >> x3;
    std::cout << "max: " << max(x1, x2, x3) << std::endl;
    std::cout << "min: " << min(x1, x2, x3) << std::endl;

    Vector_t vec{x1, x2, x3};
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
