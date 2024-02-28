#include "first_app.hpp"

#include <memory>
#include <vector>
#include <stdexcept>

namespace yLab
{

FirstApp::FirstApp()
{
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
    }
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
}

void FirstApp::drawFrame()
{
}

} // namespace yLab
