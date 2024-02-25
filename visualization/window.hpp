#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace yLab
{

class Window
{
public:
    Window(int w, int h, std::string name) : width{w}, height{h}, window_name{name}
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(w, h, window_name.c_str(), nullptr, nullptr);
    }

    ~Window()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool shouldClose()
    {
        return glfwWindowShouldClose(window);
    }

private:
    const int width;
    const int height;

    std::string window_name;
    GLFWwindow* window;

};


} // namespace yLab
