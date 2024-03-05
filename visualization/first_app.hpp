#include "window.hpp"
#include "pipeline.hpp"
#include "device.hpp"
#include "swap_chain.hpp"
#include "object.hpp"

#include <memory>
#include <string>

namespace yLab
{

class FirstApp
{
public:
    static const int WIDTH = 1000;
    static const int HEIGHT = 800;

    FirstApp();
    ~FirstApp();

    FirstApp(const FirstApp&) = delete;
    FirstApp& operator=(const FirstApp&) = delete;

    void run();

    std::string shaders_path = SHADERS_PATH; // defined at cmake configure time

private:
    void loadObjects();
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void freeCommandBuffers();
    void drawFrame();
    void recreateSwapChain();
    void recordCommandBuffer(int image_index);
    void renderObjects(VkCommandBuffer command_buffer);

    Window window{WIDTH, HEIGHT, "Hello"};
    Device device{window};

    std::unique_ptr<SwapChain> swap_chain;
    std::unique_ptr<Pipeline> pipeline;

    VkPipelineLayout pipeline_layout;

    std::vector<VkCommandBuffer> command_buffers;
    std::vector<Object> objects;
};

} // namespace yLab
