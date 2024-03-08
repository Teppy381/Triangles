#include "first_app.hpp"

#include "render_system.hpp"
#include "camera.hpp"
#include "keyboard_controller.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <memory>
#include <array>
#include <vector>
#include <stdexcept>
#include <chrono>

namespace yLab
{

FirstApp::FirstApp(std::vector<geometry::Boxed_triangle_t>& triangles_, std::vector<bool>& intersection_list_)
    : triangles{triangles_}, intersection_list{intersection_list_}
{
    loadObjects();
}

void FirstApp::keyCallback(GLFWwindow* window_, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window_, GLFW_TRUE);
    }
}

void FirstApp::scrollCallback(GLFWwindow* window_, double x_offset, double y_offset)
{
    // somehow call moveTowardsTarget and moveAroundTarget
}

void FirstApp::run()
{
    RenderSystem render_system{device, renderer.getSwapChainRenderPass()};
    Camera camera{};

    auto viewerObject = Object::createObject();
    viewerObject.transform.translation = {1, -1, 0.0};
    KeyboardController camera_controller{};

    auto current_time = std::chrono::high_resolution_clock::now();

    glfwShowWindow(window.getGLFWwindow());
    while (!window.shouldClose())
    {
        glfwPollEvents();

        auto new_time = std::chrono::high_resolution_clock::now();
        float frame_time
            = std::chrono::duration<float, std::chrono::seconds::period>(new_time - current_time).count();
        current_time = new_time;

        const float MAX_FRAME_TIME = 0.1;
        frame_time = glm::min(frame_time, MAX_FRAME_TIME);

        // std::cout << "frame_time: " << frame_time << std::endl;

        const glm::vec3 target = {0.0f, 0.0f, 0.0f};
        camera_controller.moveTowardsTarget(window.getGLFWwindow(), frame_time, viewerObject, target);
        camera_controller.moveAroundTarget(window.getGLFWwindow(), frame_time, viewerObject, target);

        camera.setViewTarget(viewerObject.transform.translation, target);

        float aspect = renderer.getAspectRatio();
        camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 100.0f);
        auto command_buffer = renderer.beginFrame();
        if (command_buffer != nullptr)
        {
            // more stuff

            renderer.beginSwapChainRenderPass(command_buffer);
            render_system.renderObjects(command_buffer, objects, camera);
            renderer.endSwapChainRenderPass(command_buffer);
            renderer.endFrame();
        }
    }

    std::cout << "Closing window..." << std::endl;
    vkDeviceWaitIdle(device.device());
}

// temporary helper function, creates a 1x1x1 cube centered at offset
std::unique_ptr<Model> createCubeModel(Device& device, glm::vec3 offset)
{
    std::vector<Model::Vertex> vertices{

        // left face (white)
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

        // right face (yellow)
        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

        // top face (orange, remember y axis points down)
        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

        // bottom face (red)
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

        // nose face (blue)
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

        // tail face (green)
        {{-.5f, -.5f, -.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -.5f}, {.1f, .8f, .1f}},
        {{-.5f, .5f, -.5f}, {.1f, .8f, .1f}},
        {{-.5f, -.5f, -.5f}, {.1f, .8f, .1f}},
        {{.5f, -.5f, -.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -.5f}, {.1f, .8f, .1f}},

    };
    for (auto& v : vertices)
    {
        v.position += offset;
    }
    return std::make_unique<Model>(device, vertices);
}

void FirstApp::loadObjects()
{
    std::shared_ptr<Model> model = createCubeModel(device, {0.0f, 0.0f, 0.0f});

    Object cube = Object::createObject();
    cube.model = model;
    cube.transform.translation = {0.0f, 0.0f, 0.0f};
    cube.transform.scale = {0.01f, 0.01f, 0.01f};
    cube.transform.rotation = {0.0f, 0.0f, 0.0f};
    objects.push_back(std::move(cube));


    const glm::vec3 red_color = {0.8f, 0.1f, 0.1f};
    const glm::vec3 blue_color = {0.1f, 0.1f, 0.8f};

    for (int i = 0; i < triangles.size(); ++i)
    {
        glm::vec3 color{};

        if (intersection_list[i] == true)
        {
            color = red_color;
        }
        else
        {
            color = blue_color;
        }

        // geometry::Vector_t normal_ = triangles[i].get_normal();
        // glm::vec3 normal = {normal_.y, normal_.z, normal_.x};

        geometry::Point_t T1 = triangles[i].P;
        geometry::Point_t T2 = triangles[i].e1 + triangles[i].P;
        geometry::Point_t T3 = triangles[i].e2 + triangles[i].P;

        std::vector<Model::Vertex> vertices{
            {{-T1.z, T1.x, T1.y}, color},
            {{-T2.z, T2.x, T2.y}, color},
            {{-T3.z, T3.x, T3.y}, color}
        };

        Object triangle = Object::createObject();
        triangle.model = std::make_unique<Model>(device, vertices);
        triangle.transform.scale = {0.01f, 0.01f, 0.01f};
        triangle.transform.rotation = {0.0f, 0.0f, 0.0f};
        objects.push_back(std::move(triangle));
    }
}

} // namespace yLab
