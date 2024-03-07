#include "first_app.hpp"
#include "input.hpp"

int main()
{

{   // for some reason this part needs additional scope, otherwise my_app does not launch properly
    std::cout << "Enter triangles:" << std::endl;
    std::vector<geometry::Boxed_triangle_t> triangles = geometry::scan_triangles();
    std::vector<bool> intersection_list = geometry::check_for_intersections(triangles);
}

    std::cout << "Launching window..." << std::endl;

    yLab::FirstApp my_app{};
    my_app.run();
    my_app.~FirstApp();
}
