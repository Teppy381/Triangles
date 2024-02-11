#include <iostream>
#include "geometry.hpp"
#include <vector>
#include <memory>

int main()
{
    size_t count;
    std::cin >> count;
    std::unique_ptr<int[]> intersection_count{new int[count]};

    std::vector<geometry::Boxed_triangle_t> triangles;

    for(size_t i = 0; i != count; ++i)
    {
        intersection_count[i] = 0;
        triangles.push_back(geometry::scan_boxed_triangle());
    }
    for(size_t i = 0; i != count; ++i)
    {
        for(size_t j = i + 1; j != count; ++j)
        {
            if (geometry::lookup_intersection(triangles[i], triangles[j]))
            {
                intersection_count[i] += 1;
                intersection_count[j] += 1;
            }
        }
    }

    bool found_intersection = false;
    for(size_t i = 0; i != count; ++i)
    {
        if (intersection_count[i] > 0)
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