#include "window.hpp"
#include "pipeline.hpp"

std::string shaders_path = SHADERS_PATH; // defines at cmake configure time

int main()
{
    yLab::Window my_window{1000, 800, "Hello"};
    std::cout << shaders_path + "simple.vert.spv" << std::endl;
    yLab::Pipeline my_pipeline{shaders_path + "simple.vert.spv", shaders_path + "simple.frag.spv"};

    while(!my_window.shouldClose())
    {
        glfwPollEvents();
    }
}
