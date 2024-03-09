#pragma once

#include "window.hpp"
#include "device.hpp"
#include "swap_chain.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace yLab
{

class Renderer
{
public:
    Renderer(Window& window_, Device& device_);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    VkRenderPass getSwapChainRenderPass() const
    {
        return swap_chain->getRenderPass();
    }

    float getAspectRatio() const
    {
        return swap_chain->extentAspectRatio();
    }

    bool isFrameInProgress() const
    {
        return is_frame_started;
    }

    VkCommandBuffer getCurrentCommandBuffer() const
    {
        assert(is_frame_started && "Cannot get command buffer when frame not in progress");
        return command_buffers[current_frame_index];
    }

    int getFrameIndex() const
    {
        assert(is_frame_started && "Cannot get frame index when frame not in progress");
        return current_frame_index;
    }

    // beginFrame and beginSwapChainRenderPass are not combined because we might want multiple RenderPasses
    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer command_buffer);
    void endSwapChainRenderPass(VkCommandBuffer command_buffer);

private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();

    Window& window;
    Device& device;

    std::unique_ptr<SwapChain> swap_chain;
    std::vector<VkCommandBuffer> command_buffers;

    uint32_t current_image_index;
    int current_frame_index = 0;
    bool is_frame_started = false;
};

} // namespace yLab
