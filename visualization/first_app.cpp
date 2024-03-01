#include "first_app.hpp"

#include <memory>
#include <array>
#include <vector>
#include <stdexcept>

namespace yLab
{

FirstApp::FirstApp()
{
    loadModels();
    createPipelineLayout();
    createPipeline();
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

void FirstApp::loadModels()
{
    std::vector<Model::Vertex> vertices{
        {{0.0f, -0.5f}},
        {{0.5f, 0.5f}},
        {{-0.5f, 0.5f}}
    };

    model = std::make_unique<Model>(device, vertices);
}

void FirstApp::createPipelineLayout()
{
    VkPipelineLayoutCreateInfo pipeline_layout_info{};
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_info.setLayoutCount = 0;
    pipeline_layout_info.pSetLayouts = nullptr;
    pipeline_layout_info.pushConstantRangeCount = 0;
    pipeline_layout_info.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(device.device(), &pipeline_layout_info, nullptr, &pipeline_layout)
        != VK_SUCCESS)
    {
        throw std::runtime_error{"Failed to create pipeline layout!"};
    }
}

void FirstApp::createPipeline()
{
    auto pipeline_config = Pipeline::defaultPipelineConfigInfo(swap_chain.width(), swap_chain.height());
    pipeline_config.render_pass = swap_chain.getRenderPass();
    pipeline_config.pipeline_layout = pipeline_layout;
    pipeline = std::make_unique<Pipeline>(
        device, shaders_path + "simple.vert.spv", shaders_path + "simple.frag.spv", pipeline_config
    );
}

void FirstApp::createCommandBuffers()
{
    command_buffers.resize(swap_chain.imageCount());

    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandPool = device.getCommandPool();
    alloc_info.commandBufferCount = static_cast<uint32_t>(command_buffers.size());

    if (vkAllocateCommandBuffers(device.device(), &alloc_info, command_buffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error{"Failed to allocate command buffers!"};
    }
    for (int i = 0; i < command_buffers.size(); i++)
    {
        VkCommandBufferBeginInfo begin_info{};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(command_buffers[i], &begin_info) != VK_SUCCESS)
        {
            throw std::runtime_error{"Failed to begin recording command buffer"};
        }

        VkRenderPassBeginInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = swap_chain.getRenderPass();
        render_pass_info.framebuffer = swap_chain.getFrameBuffer(i);

        render_pass_info.renderArea.offset = {0, 0};
        render_pass_info.renderArea.extent = swap_chain.getSwapChainExtent();

        std::array<VkClearValue, 2> clear_values{};
        clear_values[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clear_values[1].depthStencil = {1.0f, 0};
        render_pass_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
        render_pass_info.pClearValues = clear_values.data();

        vkCmdBeginRenderPass(command_buffers[i], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

        pipeline->bind(command_buffers[i]);
        model->bind(command_buffers[i]);
        model->draw(command_buffers[i]);

        vkCmdEndRenderPass(command_buffers[i]);
        if (vkEndCommandBuffer(command_buffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error{"Failed to record command buffer"};
        }
    }
}

void FirstApp::drawFrame()
{
    uint32_t image_index;

    auto result = swap_chain.acquireNextImage(&image_index);
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error{"Failed to acquire swap chain image"};
    }

    result = swap_chain.submitCommandBuffers(&command_buffers[image_index], &image_index);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error{"Failed to present swap chain image"};
    }
}

} // namespace yLab
