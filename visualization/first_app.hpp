#pragma once

#include "window.hpp"
#include "device.hpp"
#include "renderer.hpp"
#include "object.hpp"

#include "input.hpp"

#include <memory>
#include <string>
#include <vector>

namespace yLab
{

enum CameraMods : int
{
    aroundCam = 0,
    freeCam = 1
};

class FirstApp
{
public:
    static const int WIDTH = 1000;
    static const int HEIGHT = 800;

    FirstApp(std::vector<geometry::Boxed_triangle_t>& triangles_, std::vector<bool>& intersection_list_);
    ~FirstApp() = default;

    FirstApp(const FirstApp&) = delete;
    FirstApp& operator=(const FirstApp&) = delete;

    void run();

private:
    void loadObjects();
    void loadTestObjects();

    static void keyCallback(GLFWwindow* window_, int key, int scancode, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double x_offset, double y_offset);

    std::vector<geometry::Boxed_triangle_t> triangles;
    std::vector<bool> intersection_list;

    Window window{WIDTH, HEIGHT, "Triangles", keyCallback, scrollCallback};
    Device device{window};
    Renderer renderer{window, device};
    std::vector<Object> objects;

    const std::vector<int> camera_modes = {aroundCam, freeCam};

    const glm::vec3 home_camera_position = {1.0f, -0.5f, 1.0f};
};

} // namespace yLab
