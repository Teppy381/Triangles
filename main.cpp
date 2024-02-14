#include <iostream>
#include "input.hpp"
#include <vector>
#include <memory>

int main()
{
    std::vector<geometry::Boxed_triangle_t> triangles = geometry::scan_triangles();
    size_t count = triangles.size();

    std::vector<bool> intersection_list = geometry::check_for_intersections(triangles);
    assert(count == intersection_list.size());

    bool found_intersection = false;
    for(size_t i = 0; i != count; ++i)
    {
        if (intersection_list[i] > 0)
        {
            std::cout << i << " ";
            found_intersection = true;
        }
    }
    if (!found_intersection)
    {
        std::cout << "No intersections found";

    }
    std::cout << std::endl;

    return 0;
}