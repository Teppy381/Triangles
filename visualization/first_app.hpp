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

    Window window{WIDTH, HEIGHT, "Hello"};
    Device device{window};
    Renderer renderer{window, device};
    std::vector<Object> objects;
};

} // namespace yLab
