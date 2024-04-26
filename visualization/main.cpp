#include "first_app.hpp"

int main()
{
    // For some reason this part needs additional scope when compiling with "-O0" and
    // there is no use of triangles or intersection_list before FirstApp::FirstApp() is called
    // otherwise ERROR: vkBeginCommandBuffer: Invalid commandBuffer [VUID-vkBeginCommandBuffer-commandBuffer-parameter]
    // {
    std::cout << "Enter triangles:" << std::endl;

    std::vector<geometry::Boxed_triangle_t> triangles = geometry::scan_triangles();
    std::vector<bool> intersection_list = geometry::check_for_intersections(triangles);
    // }

    std::cout << "Launching window..." << std::endl;

    yLab::FirstApp my_app{triangles, intersection_list};
    my_app.run();
}
