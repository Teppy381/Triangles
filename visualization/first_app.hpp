#pragma once

#include "window.hpp"
#include "device.hpp"
#include "renderer.hpp"
#include "object.hpp"

#include <memory>
#include <string>
#include <vector>

namespace yLab
{

class FirstApp
{
public:
    static const int WIDTH = 1000;
    static const int HEIGHT = 800;

    FirstApp();
    ~FirstApp() = default;

    FirstApp(const FirstApp&) = delete;
    FirstApp& operator=(const FirstApp&) = delete;

    void run();

private:
    void loadObjects();

    static void keyCallback(GLFWwindow* window_, int key, int scancode, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double x_offset, double y_offset);

    Window window{WIDTH, HEIGHT, "Hello", keyCallback, scrollCallback};
    Device device{window};
    Renderer renderer{window, device};
    std::vector<Object> objects;
};

} // namespace yLab
