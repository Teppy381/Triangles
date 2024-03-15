#pragma once

#include "object.hpp"

namespace yLab
{

struct CameraController
{
    CameraController(GLFWwindow* window_) : window{window_}
    {}

    struct KeyMappings
    {
        int moveLeft = GLFW_KEY_A;
        int moveRight = GLFW_KEY_D;
        int moveForward = GLFW_KEY_W;
        int moveBackward = GLFW_KEY_S;
        int moveUp = GLFW_KEY_SPACE;
        int moveDown = GLFW_KEY_LEFT_ALT;
        int lookLeft = GLFW_KEY_LEFT;
        int lookRight = GLFW_KEY_RIGHT;
        int lookUp = GLFW_KEY_UP;
        int lookDown = GLFW_KEY_DOWN;
        int accelerate = GLFW_KEY_LEFT_SHIFT;
        int goHome = GLFW_KEY_BACKSPACE;
        int CamMode0 = GLFW_KEY_1;
        int CamMode1 = GLFW_KEY_2;
    };

    struct CameraData
    {
        glm::vec3 translation;
        glm::vec3 u = {1.0f, 0.0f, 0.0f};
        glm::vec3 v = {0.0f, 1.0f, 0.0f};
        glm::vec3 w = {0.0f, 0.0f, 1.0f};

        int movement_mode = 0;
    };

    GLFWwindow* window;
    KeyMappings keys{};
    CameraData camera_data{};
    float move_speed = 1.0f;
    float look_speed = 1.5f;
    float rotation_speed = 1.5f;
    float move_acceleration_multiplier = 4.0f;
    float look_acceleration_multiplier = 2.0f;
    float rotation_acceleration_multiplier = 2.0f;

    void swichCamMode(const std::vector<int> camera_mods);
    void moveHome(glm::vec3 home_position);

    void moveTowardsTarget(float dt, glm::vec3 target);
    void moveAroundTarget(float dt, glm::vec3 target);
    void lookOnTarget(glm::vec3 target, glm::vec3 up = {0.0f, -1.0f, 0.0f});
    void lookInDirection(glm::vec3 direction, glm::vec3 up = {0.0f, -1.0f, 0.0f});

    void moveInPlaneXZ(float dt);
};

}
