#include "first_app.hpp"
#include "render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <memory>
#include <array>
#include <vector>
#include <stdexcept>

namespace yLab
{

FirstApp::FirstApp()
{
    loadObjects();
}

void FirstApp::run()
{
    RenderSystem render_system{device, renderer.getSwapChainRenderPass()};

    while (!window.shouldClose())
    {
        glfwPollEvents();

        auto command_buffer = renderer.beginFrame();
        if (command_buffer != nullptr)
        {
            // more stuff

            renderer.beginSwapChainRenderPass(command_buffer);
            render_system.renderObjects(command_buffer, objects);
            renderer.endSwapChainRenderPass(command_buffer);
            renderer.endFrame();
        }
    }

    vkDeviceWaitIdle(device.device());
}

void FirstApp::loadObjects()
{
    std::vector<Model::Vertex> vertices{
        {{0.0f, -0.5f}, {1.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}}
    };

    auto model = std::make_shared<Model>(device, vertices);

    auto triangle = Object::createObject();
    triangle.model = model;
    triangle.color = {0.1f, 0.8f, 0.1f};
    triangle.transform2d.translation.x = 0.3f;
    triangle.transform2d.scale = {2.0f, 0.5f};
    triangle.transform2d.rotation = 0.2f * glm::two_pi<float>();

    objects.push_back(std::move(triangle));
}

} // namespace yLab
