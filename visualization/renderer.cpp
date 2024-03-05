#include "renderer.hpp"

#include <memory>
#include <array>
#include <vector>
#include <stdexcept>

namespace yLab
{

Renderer::Renderer(Window& window_, Device& device_) : window{window_}, device{device_}
{
    recreateSwapChain();
    createCommandBuffers();
}

Renderer::~Renderer()
{
    freeCommandBuffers();
}

void Renderer::recreateSwapChain()
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
        std::shared_ptr<SwapChain> old_swap_chain = std::move(swap_chain);
        swap_chain = std::make_unique<SwapChain>(device, extent, old_swap_chain);

        if (!old_swap_chain->compareSwapFormats(*swap_chain.get()))
        {
            throw std::runtime_error{"Swap chain image (or depth) has changed!"};
        }

        // Probably not needed
        // if (swap_chain->imageCount() != command_buffers.size())
        // {
        //     freeCommandBuffers();
        //     createCommandBuffers();
        // }
    }
}

void Renderer::createCommandBuffers()
{
    command_buffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

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

void Renderer::freeCommandBuffers()
{
    vkFreeCommandBuffers(
        device.device(),
        device.getCommandPool(),
        static_cast<uint32_t>(command_buffers.size()),
        command_buffers.data()
    );
    command_buffers.clear();
}

VkCommandBuffer Renderer::beginFrame()
{
    assert(!is_frame_started && "Can't call beginFrame while already in progress");

    auto result = swap_chain->acquireNextImage(&current_image_index);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreateSwapChain();
        return nullptr;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    is_frame_started = true;

    auto command_buffer = getCurrentCommandBuffer();
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(command_buffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    return command_buffer;
}

void Renderer::endFrame()
{
    assert(is_frame_started && "Can't call endFrame while frame is not in progress");

    auto command_buffer = getCurrentCommandBuffer();
    if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer!");
    }

    auto result = swap_chain->submitCommandBuffers(&command_buffer, &current_image_index);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized())
    {
        window.resetWindowResizedFlag();
        recreateSwapChain();
    }
    else if (result != VK_SUCCESS)
    {
        throw std::runtime_error{"Failed to present swap chain image"};
    }

    is_frame_started = false;
    current_frame_index = (current_frame_index + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
}

void Renderer::beginSwapChainRenderPass(VkCommandBuffer command_buffer)
{
    assert(is_frame_started && "Can't call beginSwapChainRenderPass if frame is not in progress");
    assert(
        command_buffer == getCurrentCommandBuffer()
        && "Can't begin render pass on command buffer from a different frame"
    );

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = swap_chain->getRenderPass();
    renderPassInfo.framebuffer = swap_chain->getFrameBuffer(current_image_index);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swap_chain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(command_buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swap_chain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(swap_chain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, swap_chain->getSwapChainExtent()};
    vkCmdSetViewport(command_buffer, 0, 1, &viewport);
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);
}

void Renderer::endSwapChainRenderPass(VkCommandBuffer command_buffer)
{
    assert(is_frame_started && "Can't call endSwapChainRenderPass if frame is not in progress");
    assert(
        command_buffer == getCurrentCommandBuffer()
        && "Can't end render pass on command buffer from a different frame"
    );
    vkCmdEndRenderPass(command_buffer);
}

} // namespace yLab
