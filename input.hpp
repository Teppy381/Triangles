#pragma once

#include <iostream>
#include "geometry.hpp"
#include <vector>
#include <memory>
#include <stdexcept>


namespace geometry
{

Point_t scan_point()
{
    float x, y, z;
    std::cin >> x >> y >> z;
    if (std::cin.fail())
    {
        // std::cout << x << ", " << y << ", " << z << std::endl;
        throw std::runtime_error{"Cannot read coordinates"};
    }
    return Point_t{x, y, z};
}

Boxed_triangle_t scan_boxed_triangle()
{
    return Boxed_triangle_t{scan_point(), scan_point(), scan_point()};
}

std::vector<geometry::Boxed_triangle_t> scan_triangles()
{
    size_t count;
    std::cin >> count;

    std::vector<geometry::Boxed_triangle_t> triangles;
    for (size_t i = 0; i < count; ++i)
    {
        triangles.push_back(scan_boxed_triangle());
    }
    return triangles;
}

// return_value[i] is true when triangles[i] intersects other triangle from the triangles
std::vector<bool> check_for_intersections(const std::vector<Boxed_triangle_t>& triangles)
{
    size_t count = triangles.size();

    std::vector<bool> intersection_list;
    for(int i = 0; i < count; ++i)
    {
        intersection_list.emplace_back(false);
    }
    assert(intersection_list.size() == triangles.size());

    for(size_t i = 0; i < count; ++i)
    {
        for(size_t j = i + 1; j < count; ++j)
        {
            if (lookup_intersection(triangles[i], triangles[j]))
            {
                intersection_list[i] = true;
                intersection_list[j] = true;
            }
        }
    }
    return intersection_list;
}

// return_value[i] is true when triangles[tr_num] intersects triangles[i]
std::vector<bool> check_for_intersections(const std::vector<Boxed_triangle_t>& triangles, size_t tr_num)
{
    size_t count = triangles.size();
    if (tr_num >= count)
    {
        throw std::out_of_range("check_for_intersections: tr_num >= triangles.size()");
    }
    std::vector<bool> intersection_list;
    for(int i = 0; i < count; ++i)
    {
        intersection_list.emplace_back(false);
    }
    assert(intersection_list.size() == triangles.size());

    for(size_t i = 0; i < tr_num; ++i)
    {
        intersection_list[i] = lookup_intersection(triangles[tr_num], triangles[i]);
    }
    intersection_list[tr_num] = true;
    for(size_t i = tr_num + 1; i < count; ++i)
    {
        intersection_list[i] = lookup_intersection(triangles[tr_num], triangles[i]);
    }
    return intersection_list;
}

} // namespace geometry
