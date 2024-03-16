#pragma once

#include "device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace yLab{


class Model
{
public:

    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 normal;

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

    Model(Device& device_, const std::vector<Vertex>& vertices);
    ~Model();

    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    void bind(VkCommandBuffer command_buffer);
    void draw(VkCommandBuffer command_buffer);

private:
    void createVertexBuffers(const std::vector<Vertex>& vertices);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    Device& device;
    VkBuffer vertex_buffer;
    VkDeviceMemory vertex_buffer_memory;
    uint32_t vertex_count;
};

}
