#include "window.hpp"
#include "pipeline.hpp"
#include "device.hpp"
#include "swap_chain.hpp"

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
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();

    Window window{WIDTH, HEIGHT, "Hello"};
    Device device{window};
    SwapChain swap_chain{device, window.getExtent()};

    std::unique_ptr<Pipeline> pipeline;

    VkPipelineLayout pipeline_layout;

    std::vector<VkCommandBuffer> command_buffers;
};

} // namespace yLab
