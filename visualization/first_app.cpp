#include "first_app.hpp"

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

struct SimplePushConstantData
{
    glm::mat2 transform{1.0f};
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
};

FirstApp::FirstApp()
{
    loadObjects();
    createPipelineLayout();
    recreateSwapChain();
    createCommandBuffers();
}

FirstApp::~FirstApp()
{
    vkDestroyPipelineLayout(device.device(), pipeline_layout, nullptr);
}

void FirstApp::run()
{
    while (!window.shouldClose())
    {
        glfwPollEvents();
        drawFrame();
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

void FirstApp::createPipelineLayout()
{

    VkPushConstantRange push_constant_range{};
    push_constant_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    push_constant_range.offset = 0;
    push_constant_range.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipeline_layout_info{};
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_info.setLayoutCount = 0;
    pipeline_layout_info.pSetLayouts = nullptr;
    pipeline_layout_info.pushConstantRangeCount = 1;
    pipeline_layout_info.pPushConstantRanges = &push_constant_range;

    if (vkCreatePipelineLayout(device.device(), &pipeline_layout_info, nullptr, &pipeline_layout)
        != VK_SUCCESS)
    {
        throw std::runtime_error{"Failed to create pipeline layout!"};
    }
}

void FirstApp::createPipeline()
{
    assert(swap_chain != nullptr);
    assert(pipeline_layout != nullptr);

    PipelineConfigInfo pipeline_config{};
    Pipeline::defaultPipelineConfigInfo(pipeline_config);

    pipeline_config.render_pass = swap_chain->getRenderPass();
    pipeline_config.pipeline_layout = pipeline_layout;
    pipeline = std::make_unique<Pipeline>(
        device, shaders_path + "simple.vert.spv", shaders_path + "simple.frag.spv", pipeline_config
    );
}

void FirstApp::recreateSwapChain()
{
    auto extent = window.getExtent();
    while (extent.width == 0 || extent.height == 0) // pause app if some side is 0
    {
        extent = window.getExtent();
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(device.device()); // wait until current swap_chain is no longer used

    // swap_chain = nullptr;
    if (swap_chain == nullptr)
    {
        swap_chain = std::make_unique<SwapChain>(device, extent);
    }
    else
    {
        swap_chain = std::make_unique<SwapChain>(device, extent, std::move(swap_chain));
        if (swap_chain->imageCount() != command_buffers.size())
        {
            freeCommandBuffers();
            createCommandBuffers();
        }
    }

    // if render pass compatible do nothing
    createPipeline();
}

void FirstApp::createCommandBuffers()
{
    command_buffers.resize(swap_chain->imageCount());

    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandPool = device.getCommandPool();
    alloc_info.commandBufferCount = static_cast<uint32_t>(command_buffers.size());

    if (vkAllocateCommandBuffers(device.device(), &alloc_info, command_buffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error{"Failed to allocate command buffers!"};
    }
    return;
}

void FirstApp::freeCommandBuffers()
{
    vkFreeCommandBuffers(
        device.device(),
        device.getCommandPool(),
        static_cast<uint32_t>(command_buffers.size()),
        command_buffers.data()
    );
    command_buffers.clear();
}

void FirstApp::recordCommandBuffer(int image_index)
{
    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(command_buffers[image_index], &begin_info) != VK_SUCCESS)
    {
        throw std::runtime_error{"Failed to begin recording command buffer"};
    }

    VkRenderPassBeginInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass = swap_chain->getRenderPass();
    render_pass_info.framebuffer = swap_chain->getFrameBuffer(image_index);

    render_pass_info.renderArea.offset = {0, 0};
    render_pass_info.renderArea.extent = swap_chain->getSwapChainExtent();

    std::array<VkClearValue, 2> clear_values{};
    clear_values[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
    clear_values[1].depthStencil = {1.0f, 0};
    render_pass_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
    render_pass_info.pClearValues = clear_values.data();

    vkCmdBeginRenderPass(command_buffers[image_index], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swap_chain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(swap_chain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, swap_chain->getSwapChainExtent()};
    vkCmdSetViewport(command_buffers[image_index], 0, 1, &viewport);
    vkCmdSetScissor(command_buffers[image_index], 0, 1, &scissor);

    renderObjects(command_buffers[image_index]);

    vkCmdEndRenderPass(command_buffers[image_index]);
    if (vkEndCommandBuffer(command_buffers[image_index]) != VK_SUCCESS)
    {
        throw std::runtime_error{"Failed to record command buffer"};
    }
}

void FirstApp::renderObjects(VkCommandBuffer command_buffer)
{
    pipeline->bind(command_buffer);

    for (auto&& obj : objects)
    {
        obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f, glm::two_pi<float>());

        SimplePushConstantData push{};
        push.offset = obj.transform2d.translation;
        push.color = obj.color;
        push.transform = obj.transform2d.mat2();

        vkCmdPushConstants(
            command_buffer,
            pipeline_layout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(SimplePushConstantData),
            &push
        );
        obj.model->bind(command_buffer);
        obj.model->draw(command_buffer);
    }
}

void FirstApp::drawFrame()
{
    uint32_t image_index;

    auto result = swap_chain->acquireNextImage(&image_index);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreateSwapChain();
        return;
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error{"Failed to acquire swap chain image"};
    }

    recordCommandBuffer(image_index);
    result = swap_chain->submitCommandBuffers(&command_buffers[image_index], &image_index);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized())
    {
        window.resetWindowResizedFlag();
        recreateSwapChain();
        return;
    }
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error{"Failed to present swap chain image"};
    }
}

} // namespace yLab
