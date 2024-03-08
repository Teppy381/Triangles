#pragma once

#include "object.hpp"

namespace yLab
{

class KeyboardController
{
public:
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
    };

    KeyMappings keys{};
    float move_speed = 2.0f;
    float look_speed = 1.5f;
    float rotation_speed = 2.0f;

    void moveInPlaneXZ(GLFWwindow* window, float dt, Object& object);

    void moveTowardsTarget(GLFWwindow* window, float dt, Object& object, glm::vec3 target);
    void moveAroundTarget(GLFWwindow* window, float dt, Object& object, glm::vec3 target);
};

}
