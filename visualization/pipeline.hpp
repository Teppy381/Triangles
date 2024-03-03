#pragma once

#include "device.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace yLab
{


struct PipelineConfigInfo
{
    PipelineConfigInfo(const PipelineConfigInfo&) = delete;
    PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

    VkPipelineViewportStateCreateInfo viewport_info;
    VkPipelineInputAssemblyStateCreateInfo input_assembly_info;
    VkPipelineRasterizationStateCreateInfo rasterization_info;
    VkPipelineMultisampleStateCreateInfo multisample_info;
    VkPipelineColorBlendAttachmentState color_blend_attachment;
    VkPipelineColorBlendStateCreateInfo color_blend_info;
    VkPipelineDepthStencilStateCreateInfo depth_stencil_info;
    std::vector<VkDynamicState> dynamic_state_enables;
    VkPipelineDynamicStateCreateInfo dynamic_state_info;
    VkPipelineLayout pipeline_layout = nullptr;
    VkRenderPass render_pass = nullptr;
    uint32_t subpass = 0;
};


class Pipeline
{
public:
    Pipeline(Device& device_, const std::string& vert_filepath,
             const std::string& frag_filepath, const PipelineConfigInfo& config_info);
    // Pipeline() = default;

    ~Pipeline();

    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    void bind(VkCommandBuffer command_buffer);

    static void defaultPipelineConfigInfo(PipelineConfigInfo& config_info);

private:

    Device& device;
    VkPipeline graphics_pipeline;
    VkShaderModule vert_shader_module;
    VkShaderModule frag_shader_module;

    std::vector<char> readFile(const std::string& filepath);

    void createShaderModule(const std::vector<char>& code, VkShaderModule* shader_module);

};

}
