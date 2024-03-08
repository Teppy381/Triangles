#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdexcept>

namespace yLab
{

class Window
{
public:
    Window(
        int w,
        int h,
        std::string name,
        GLFWkeyfun keyCallback = nullptr,
        GLFWscrollfun scrollCallback = nullptr
    )
        : width{w}, height{h}, window_name{name}
    {
        glfwSetErrorCallback(errorCallback);
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
        window = glfwCreateWindow(w, h, window_name.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
        glfwSetKeyCallback(window, keyCallback);
        glfwSetScrollCallback(window, scrollCallback);
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

    VkExtent2D getExtent()
    {
        return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
    }

    bool wasWindowResized()
    {
        return frame_buffer_resized;
    }

    void resetWindowResizedFlag()
    {
        frame_buffer_resized = false;
    }

    GLFWwindow* getGLFWwindow() const
    {
        return window;
    }

    void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
        {
            throw std::runtime_error{"Failed to create window surface"};
        }
    }

private:
    static void frameBufferResizeCallback(GLFWwindow* window_, int width, int height)
    {
        auto window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window_));
        window->frame_buffer_resized = true;
        window->width = width;
        window->height = height;
    }

    static void errorCallback(int error, const char* description)
    {
        std::cout << error << ": " << description << std::endl;
    }

    int width;
    int height;
    bool frame_buffer_resized = false;

    std::string window_name;
    GLFWwindow* window;
};

} // namespace yLab
