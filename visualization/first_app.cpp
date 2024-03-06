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

FirstApp::FirstApp()
{
    loadObjects();
}

void FirstApp::run()
{
    RenderSystem render_system{device, renderer.getSwapChainRenderPass()};
    Camera camera{};

    auto viewerObject = Object::createObject();
    viewerObject.transform.translation = {0.2, 0.0, 0.0};
    KeyboardController camera_controller{};

    auto current_time = std::chrono::high_resolution_clock::now();

    while (!window.shouldClose())
    {
        glfwPollEvents();

        auto new_time = std::chrono::high_resolution_clock::now();
        float frame_time = std::chrono::duration<float, std::chrono::seconds::period>(new_time - current_time).count();
        current_time = new_time;

        const float MAX_FRAME_TIME = 0.1;
        frame_time = glm::min(frame_time, MAX_FRAME_TIME);

        // std::cout << "frame_time: " << frame_time << std::endl;

        const glm::vec3 target = {0.0f, 0.0f, 0.5f};
        camera_controller.moveTowardsTarget(window.getGLFWwindow(), frame_time, viewerObject, target);
        camera_controller.moveAroundTarget(window.getGLFWwindow(), frame_time, viewerObject, target);

        camera.setViewTarget(viewerObject.transform.translation, target);


        float aspect = renderer.getAspectRatio();
        camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);
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

    Object cube1 = Object::createObject();
    cube1.model = model;
    cube1.transform.translation = {0.0f, 0.0f, 0.5f};
    cube1.transform.scale = {0.5f, 0.5f, 0.5f};
    objects.push_back(std::move(cube1));

    Object cube2 = Object::createObject();
    cube2.model = model;
    cube2.transform.translation = {0.0f, 0.0f, 0.0f};
    cube2.transform.scale = {0.05f, 0.05f, 0.05f};
    cube2.transform.rotation = {2.0f, 2.0f, 0.0f};
    objects.push_back(std::move(cube2));
}

} // namespace yLab
