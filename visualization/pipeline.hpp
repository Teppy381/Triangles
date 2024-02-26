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

};


class Pipeline
{
public:
    Pipeline(Device& device_, const std::string& vert_filepath,
             const std::string& frag_filepath, const PipelineConfigInfo& config_info)
    : device{device_}
    {
        auto vert_code = readFile(vert_filepath);
        auto frag_code = readFile(frag_filepath);

        std::cout << "Vertex shader code size: " << vert_code.size() << std::endl;
        std::cout << "Fragment shader code size: " << frag_code.size() << std::endl;

    }

    ~Pipeline() {}

    Pipeline(const Pipeline&) = delete;
    void operator=(const Pipeline&) = delete;

    static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height)
    {
        PipelineConfigInfo config_info{};

        return config_info;
    }

private:

    Device& device;
    VkPipeline graphics_pipeline;
    VkShaderModule vert_shader_module;
    VkShaderModule frag_shader_module;

    std::vector<char> readFile(const std::string& filepath)
    {
        std::ifstream file{filepath, std::ios::ate | std::ios::binary};

        if (!file.is_open())
        {
            throw std::runtime_error{"Cannot open file: " + filepath};
        }

        size_t file_size = (size_t) file.tellg();
        std::vector<char> buffer(file_size);

        file.seekg(0);
        file.read(buffer.data(), file_size);

        file.close();
        return buffer;
    }

    void createShaderModule(const std::vector<char>& code, VkShaderModule* shader_module)
    {
        VkShaderModuleCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = code.size();
        create_info.pCode = reinterpret_cast<const uint32_t*>(code.data()); // already aligned by vector

        if (vkCreateShaderModule(device.device(), &create_info, nullptr, shader_module) != VK_SUCCESS)
        {
            throw std::runtime_error{"Failed to create shader module"};
        }
    }


};

}
