#include "first_app.hpp"

#include "render_system.hpp"
#include "camera.hpp"
#include "camera_controller.hpp"

// #define VMA_IMPLEMENTATION
// #include "vk_mem_alloc.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <memory>
#include <array>
#include <vector>
#include <stdexcept>
#include <chrono>

namespace yLab
{

FirstApp::FirstApp(std::vector<geometry::Boxed_triangle_t>& triangles_, std::vector<bool>& intersection_list_)
    : triangles{triangles_}, intersection_list{intersection_list_}
{
    loadObjects();
}

void FirstApp::keyCallback(GLFWwindow* window_, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window_, GLFW_TRUE);
    }
}

void FirstApp::scrollCallback(GLFWwindow* window_, double x_offset, double y_offset)
{
    // somehow call moveTowardsTarget and moveAroundTarget
}

void FirstApp::run()
{
    RenderSystem render_system{device, renderer.getSwapChainRenderPass()};
    Camera camera{};

    CameraController camera_controller{window.getGLFWwindow()};
    camera_controller.camera_data.translation = home_camera_position;

    auto current_time = std::chrono::high_resolution_clock::now();

    glfwShowWindow(window.getGLFWwindow());
    while (!window.shouldClose())
    {
        glfwPollEvents();

        auto new_time = std::chrono::high_resolution_clock::now();
        float frame_time
            = std::chrono::duration<float, std::chrono::seconds::period>(new_time - current_time).count();
        current_time = new_time;

        const float MAX_FRAME_TIME = 0.1;
        frame_time = glm::min(frame_time, MAX_FRAME_TIME);
        // std::cout << "frame_time: " << frame_time << std::endl;

        camera_controller.swichCamMode(camera_modes);
        camera_controller.moveHome(home_camera_position);

        if (camera_controller.camera_data.movement_mode == aroundCam)
        {
            const glm::vec3 target = {0.0f, 0.0f, 0.0f};
            camera_controller.moveTowardsTarget(frame_time, target);
            camera_controller.moveAroundTarget(frame_time, target);
            camera_controller.lookOnTarget(target);
        }
        else if (camera_controller.camera_data.movement_mode == freeCam)
        {
            camera_controller.moveInPlaneXZ(frame_time);
        }

        camera.setViewMatrix(
            camera_controller.camera_data.translation,
            camera_controller.camera_data.u,
            camera_controller.camera_data.v,
            camera_controller.camera_data.w
        );

        float aspect = renderer.getAspectRatio();
        camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 100.0f);
        auto command_buffer = renderer.beginFrame();
        if (command_buffer != nullptr)
        {
            // more stuff

            renderer.beginSwapChainRenderPass(command_buffer);
            render_system.renderObjects(command_buffer, objects, camera);
            renderer.endSwapChainRenderPass(command_buffer);
            renderer.endFrame();
        }
    }

    std::cout << "Closing window..." << std::endl;
    vkDeviceWaitIdle(device.device());
}

const glm::vec3 red_color = {0.9f, 0.05f, 0.08f};
const glm::vec3 green_color = {0.08f, 0.9f, 0.08f};
const glm::vec3 blue_color = {0.08f, 0.08f, 0.9f};
const glm::vec3 white_color = {0.9f, 0.9f, 0.9f};

const glm::vec3 light_red_color = {0.9f, 0.5f, 0.5f};
const glm::vec3 light_green_color = {0.5f, 0.9f, 0.5f};
const glm::vec3 light_blue_color = {0.5f, 0.5f, 0.9f};

const glm::vec3 x_axis = {1.0f, 0.0f, 0.0f};
const glm::vec3 y_axis = {0.0f, 0.0f, 1.0f};
const glm::vec3 z_axis = {0.0f, -1.0f, 0.0f};

// helper function, creates a 1x1x1 cube centered at offset
std::unique_ptr<Model> createCubeModel(Device& device)
{
    std::vector<Model::Vertex> vertices{

        // right face {0.5, ..., ...}
        {{.5f, -.5f, -.5f}, red_color, x_axis},
        {{.5f, -.5f, .5f}, red_color, x_axis},
        {{.5f, .5f, .5f}, red_color, x_axis},
        {{.5f, .5f, -.5f}, red_color, x_axis},
        {{.5f, -.5f, -.5f}, red_color, x_axis},
        {{.5f, .5f, .5f}, red_color, x_axis},

        // left face {-0.5, ..., ...}
        {{-.5f, -.5f, -.5f}, light_red_color, -x_axis},
        {{-.5f, .5f, .5f}, light_red_color, -x_axis},
        {{-.5f, -.5f, .5f}, light_red_color, -x_axis},
        {{-.5f, -.5f, -.5f}, light_red_color, -x_axis},
        {{-.5f, .5f, -.5f}, light_red_color, -x_axis},
        {{-.5f, .5f, .5f}, light_red_color, -x_axis},

        // nose face {..., ..., 0.5}
        {{-.5f, -.5f, .5f}, green_color, y_axis},
        {{-.5f, .5f, .5f}, green_color, y_axis},
        {{.5f, .5f, .5f}, green_color, y_axis},
        {{.5f, -.5f, .5f}, green_color, y_axis},
        {{-.5f, -.5f, .5f}, green_color, y_axis},
        {{.5f, .5f, .5f}, green_color, y_axis},

        // tail face {..., ..., -0.5}
        {{-.5f, -.5f, -.5f}, light_green_color, -y_axis},
        {{.5f, .5f, -.5f}, light_green_color, -y_axis},
        {{-.5f, .5f, -.5f}, light_green_color, -y_axis},
        {{-.5f, -.5f, -.5f}, light_green_color, -y_axis},
        {{.5f, -.5f, -.5f}, light_green_color, -y_axis},
        {{.5f, .5f, -.5f}, light_green_color, -y_axis},

        // top face (y axis points down) {..., -0.5, ...}
        {{-.5f, -.5f, -.5f}, blue_color, z_axis},
        {{-.5f, -.5f, .5f}, blue_color, z_axis},
        {{.5f, -.5f, .5f}, blue_color, z_axis},
        {{.5f, -.5f, -.5f}, blue_color, z_axis},
        {{-.5f, -.5f, -.5f}, blue_color, z_axis},
        {{.5f, -.5f, .5f}, blue_color, z_axis},

        // bottom face {..., 0.5, ...}
        {{-.5f, .5f, -.5f}, light_blue_color, -z_axis},
        {{.5f, .5f, .5f}, light_blue_color, -z_axis},
        {{-.5f, .5f, .5f}, light_blue_color, -z_axis},
        {{-.5f, .5f, -.5f}, light_blue_color, -z_axis},
        {{.5f, .5f, -.5f}, light_blue_color, -z_axis},
        {{.5f, .5f, .5f}, light_blue_color, -z_axis},
    };
    return std::make_unique<Model>(device, vertices);
}

std::unique_ptr<Model> createCubeModel(Device& device, glm::vec3 color)
{
    std::vector<Model::Vertex> vertices{

        // right face {0.5, ..., ...}
        {{.5f, -.5f, -.5f}, color, x_axis},
        {{.5f, -.5f, .5f}, color, x_axis},
        {{.5f, .5f, .5f}, color, x_axis},
        {{.5f, .5f, -.5f}, color, x_axis},
        {{.5f, -.5f, -.5f}, color, x_axis},
        {{.5f, .5f, .5f}, color, x_axis},

        // left face {-0.5, ..., ...}
        {{-.5f, -.5f, -.5f}, color, -x_axis},
        {{-.5f, .5f, .5f}, color, -x_axis},
        {{-.5f, -.5f, .5f}, color, -x_axis},
        {{-.5f, -.5f, -.5f}, color, -x_axis},
        {{-.5f, .5f, -.5f}, color, -x_axis},
        {{-.5f, .5f, .5f}, color, -x_axis},

        // nose face {..., ..., 0.5}
        {{-.5f, -.5f, .5f}, color, y_axis},
        {{-.5f, .5f, .5f}, color, y_axis},
        {{.5f, .5f, .5f}, color, y_axis},
        {{.5f, -.5f, .5f}, color, y_axis},
        {{-.5f, -.5f, .5f}, color, y_axis},
        {{.5f, .5f, .5f}, color, y_axis},

        // tail face {..., ..., -0.5}
        {{-.5f, -.5f, -.5f}, color, -y_axis},
        {{.5f, .5f, -.5f}, color, -y_axis},
        {{-.5f, .5f, -.5f}, color, -y_axis},
        {{-.5f, -.5f, -.5f}, color, -y_axis},
        {{.5f, -.5f, -.5f}, color, -y_axis},
        {{.5f, .5f, -.5f}, color, -y_axis},

        // top face (y axis points down) {..., -0.5, ...}
        {{-.5f, -.5f, -.5f}, color, z_axis},
        {{-.5f, -.5f, .5f}, color, z_axis},
        {{.5f, -.5f, .5f}, color, z_axis},
        {{.5f, -.5f, -.5f}, color, z_axis},
        {{-.5f, -.5f, -.5f}, color, z_axis},
        {{.5f, -.5f, .5f}, color, z_axis},

        // bottom face {..., 0.5, ...}
        {{-.5f, .5f, -.5f}, color, -z_axis},
        {{.5f, .5f, .5f}, color, -z_axis},
        {{-.5f, .5f, .5f}, color, -z_axis},
        {{-.5f, .5f, -.5f}, color, -z_axis},
        {{.5f, .5f, -.5f}, color, -z_axis},
        {{.5f, .5f, .5f}, color, -z_axis},
    };
    return std::make_unique<Model>(device, vertices);
}

void FirstApp::loadObjects()
{
    Object cube0 = Object::createObject();
    cube0.model = createCubeModel(device);
    cube0.transform.translation = {0.0f, 0.0f, 0.0f};
    cube0.transform.scale = {0.02f, 0.02f, 0.02f};
    cube0.transform.rotation = {0.0f, 0.0f, 0.0f};
    objects.push_back(std::move(cube0));

    Object cube_x = Object::createObject();
    cube_x.model = createCubeModel(device, red_color);
    cube_x.transform.translation = {0.1f, 0.0f, 0.0f};
    cube_x.transform.scale = {0.01f, 0.01f, 0.01f};
    cube_x.transform.rotation = {0.0f, 0.0f, 0.0f};
    objects.push_back(std::move(cube_x));

    Object cube_y = Object::createObject();
    cube_y.model = createCubeModel(device, green_color);
    cube_y.transform.translation = {0.0f, 0.0f, 0.1f};
    cube_y.transform.scale = {0.01f, 0.01f, 0.01f};
    cube_y.transform.rotation = {0.0f, 0.0f, 0.0f};
    objects.push_back(std::move(cube_y));

    Object cube_z = Object::createObject();
    cube_z.model = createCubeModel(device, blue_color);
    cube_z.transform.translation = {0.0f, -0.1f, 0.0f};
    cube_z.transform.scale = {0.01f, 0.01f, 0.01f};
    cube_z.transform.rotation = {0.0f, 0.0f, 0.0f};
    objects.push_back(std::move(cube_z));

    float triangle_scale_factor = 0;
    if (triangles.size() != 0)
    {
        glm::vec3 sum = {0, 0, 0};
        for (auto&& tr : triangles)
        {
            sum = {sum.x + tr.P_max.x, sum.y + tr.P_max.y, sum.z + tr.P_max.z};
        }

        glm::vec3 V = {triangles.size() / (5 * sqrt(glm::dot(sum, sum))), 0.002f, 0.0f};
        triangle_scale_factor = sqrt(glm::dot(V, V));
        std::cout << "Triangles scale = " << triangle_scale_factor << std::endl;
    }

    std::vector<Model::Vertex> all_triangles_vertices{};
    all_triangles_vertices.reserve(triangles.size() * 6);
    for (int i = 0; i < triangles.size(); ++i)
    {
        glm::vec3 color{};

        if (intersection_list[i] == true)
        {
            color = red_color;
        }
        else
        {
            color = blue_color;
        }

        geometry::Vector_t normal_ = triangles[i].get_normal();
        glm::vec3 normal = {normal_.x, -normal_.z, normal_.y};

        geometry::Point_t T1 = triangles[i].P1;
        geometry::Point_t T2 = triangles[i].P2;
        geometry::Point_t T3 = triangles[i].P3;

        // double vertices to show one side of triangle while the other is culled
        all_triangles_vertices.push_back({{T1.x, -T1.z, T1.y}, color, -normal});
        all_triangles_vertices.push_back({{T2.x, -T2.z, T2.y}, color, -normal});
        all_triangles_vertices.push_back({{T3.x, -T3.z, T3.y}, color, -normal});
        all_triangles_vertices.push_back({{T2.x, -T2.z, T2.y}, color, normal});
        all_triangles_vertices.push_back({{T1.x, -T1.z, T1.y}, color, normal});
        all_triangles_vertices.push_back({{T3.x, -T3.z, T3.y}, color, normal});
    }

    if (all_triangles_vertices.size() != 0)
    {
        Object all_triangles = Object::createObject();
        all_triangles.model = std::make_unique<Model>(device, all_triangles_vertices);
        all_triangles.transform.scale = {triangle_scale_factor, triangle_scale_factor, triangle_scale_factor};
        all_triangles.transform.rotation = {0.0f, 0.0f, 0.0f};
        objects.push_back(std::move(all_triangles));
    }
}

void FirstApp::loadTestObjects()
{
    Object cube0 = Object::createObject();
    cube0.model = createCubeModel(device);
    cube0.transform.translation = {0.0f, 0.0f, 0.0f};
    cube0.transform.scale = {0.02f, 0.02f, 0.02f};
    cube0.transform.rotation = {0.0f, 0.0f, 0.0f};
    objects.push_back(std::move(cube0));

    Object cube_x = Object::createObject();
    cube_x.model = createCubeModel(device, red_color);
    cube_x.transform.translation = {0.1f, 0.0f, 0.0f};
    cube_x.transform.scale = {0.01f, 0.01f, 0.01f};
    cube_x.transform.rotation = {0.0f, 0.0f, 0.0f};
    objects.push_back(std::move(cube_x));

    Object cube_y = Object::createObject();
    cube_y.model = createCubeModel(device, green_color);
    cube_y.transform.translation = {0.0f, 0.0f, 0.1f};
    cube_y.transform.scale = {0.01f, 0.01f, 0.01f};
    cube_y.transform.rotation = {0.0f, 0.0f, 0.0f};
    objects.push_back(std::move(cube_y));

    Object cube_z = Object::createObject();
    cube_z.model = createCubeModel(device, blue_color);
    cube_z.transform.translation = {0.0f, -0.1f, 0.0f};
    cube_z.transform.scale = {0.01f, 0.01f, 0.01f};
    cube_z.transform.rotation = {0.0f, 0.0f, 0.0f};
    objects.push_back(std::move(cube_z));

    glm::vec4 T1 = {4.0f, 0.0f, 0.0f, 0.0f};
    glm::vec4 T2 = {6.0f, -1.5f, 0.0f, 0.0f};
    glm::vec4 T3 = {6.0f, 1.5f, 0.0f, 0.0f};

    int N = 8;
    glm::mat4 Mat = glm::rotate(glm::mat4{1.0f}, glm::two_pi<float>() / N, {0.0f, 0.0f, 1.0f});

    std::cout << N << std::endl;

    std::vector<Model::Vertex> all_triangles_vertices{};
    for (int i = 0; i < N; ++i)
    {
        std::cout << T1.x << " " << T1.y << " " << T1.z << std::endl;
        std::cout << T2.x << " " << T2.y << " " << T2.z << std::endl;
        std::cout << T3.x << " " << T3.y << " " << T3.z << "\n\n";

        all_triangles_vertices.push_back({{T1.x, -T1.z, T1.y}, blue_color, {}});
        all_triangles_vertices.push_back({{T2.x, -T2.z, T2.y}, blue_color, {}});
        all_triangles_vertices.push_back({{T3.x, -T3.z, T3.y}, blue_color, {}});
        all_triangles_vertices.push_back({{T2.x, -T2.z, T2.y}, blue_color, {}});
        all_triangles_vertices.push_back({{T1.x, -T1.z, T1.y}, blue_color, {}});
        all_triangles_vertices.push_back({{T3.x, -T3.z, T3.y}, blue_color, {}});

        T1 = Mat * T1;
        T2 = Mat * T2;
        T3 = Mat * T3;
    }

    if (all_triangles_vertices.size() != 0)
    {
        Object all_triangles = Object::createObject();
        all_triangles.model = std::make_unique<Model>(device, all_triangles_vertices);
        all_triangles.transform.scale = {0.3f, 0.3f, 0.3f};
        all_triangles.transform.rotation = {0.0f, 0.0f, 0.0f};
        objects.push_back(std::move(all_triangles));
    }
}

} // namespace yLab
