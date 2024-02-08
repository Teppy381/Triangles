#include <iostream>
#include "geometry.hpp"

#define SHOW(...) \
    std::cout << #__VA_ARGS__ << " == " << __VA_ARGS__ << '\n'

int point_and_vector();




int main()
{
    Vector_t v1{1, 1, 1};
    Vector_t v2{1, 0, 0};
    cross_product(v1, v2).normalize().dump();
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
