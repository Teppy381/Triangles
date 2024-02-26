#include "window.hpp"
#include "pipeline.hpp"
#include "device.hpp"

std::string shaders_path = SHADERS_PATH; // defines at cmake configure time

int main()
{
    yLab::Window my_window{1000, 800, "Hello"};

    yLab::Device my_device{my_window};

    yLab::Pipeline my_pipeline{
        my_device,
        shaders_path + "simple.vert.spv",
        shaders_path + "simple.frag.spv",
        yLab::Pipeline::defaultPipelineConfigInfo(1000, 800)
    };

    while(!my_window.shouldClose())
    {
        glfwPollEvents();
    }
}
