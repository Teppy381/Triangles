#pragma once

#include "pipeline.hpp"
#include "device.hpp"
#include "object.hpp"
#include "camera.hpp"

#include <memory>
#include <string>
#include <vector>

namespace yLab
{

class RenderSystem
{
public:
    RenderSystem(Device& device_, VkRenderPass render_pass);
    ~RenderSystem();

    RenderSystem(const RenderSystem&) = delete;
    RenderSystem& operator=(const RenderSystem&) = delete;

    void renderObjects(VkCommandBuffer command_buffer, std::vector<Object>& objects, const Camera& camera);

    const std::string shaders_path = SHADERS_PATH; // defined at cmake configure time

private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass render_pass);

    Device& device;

    std::unique_ptr<Pipeline> pipeline;
    VkPipelineLayout pipeline_layout;
};

} // namespace yLab
