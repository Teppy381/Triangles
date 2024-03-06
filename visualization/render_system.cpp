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

struct SimplePushConstantData
{
    glm::mat4 transform{1.0f};
    glm::vec3 color;
};

RenderSystem::RenderSystem(Device& device_, VkRenderPass render_pass) : device{device_}
{
    createPipelineLayout();
    createPipeline(render_pass);
}

RenderSystem::~RenderSystem()
{
    vkDestroyPipelineLayout(device.device(), pipeline_layout, nullptr);
}

void RenderSystem::createPipelineLayout()
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

void RenderSystem::createPipeline(VkRenderPass render_pass)
{
    assert(pipeline_layout != nullptr);

    PipelineConfigInfo pipeline_config{};
    Pipeline::defaultPipelineConfigInfo(pipeline_config);

    pipeline_config.render_pass = render_pass;
    pipeline_config.pipeline_layout = pipeline_layout;
    pipeline = std::make_unique<Pipeline>(
        device, shaders_path + "simple.vert.spv", shaders_path + "simple.frag.spv", pipeline_config
    );
}

void RenderSystem::renderObjects(VkCommandBuffer command_buffer, std::vector<Object>& objects, const Camera& camera)
{
    pipeline->bind(command_buffer);

    auto projection_view = camera.getProjection() * camera.getView();

    for (auto&& obj : objects)
    {
        obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.0017f, glm::two_pi<float>());
        obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.0132f, glm::two_pi<float>());
        obj.transform.rotation.z = glm::mod(obj.transform.rotation.z + 0.0011f, glm::two_pi<float>());

        SimplePushConstantData push{};
        push.transform = projection_view * obj.transform.mat4();
        push.color = obj.color;

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

} // namespace yLab
