#include "keyboard_controller.hpp"
#include <limits>
#include <iostream>

namespace yLab
{

void KeyboardController::moveInPlaneXZ(GLFWwindow* window, float dt, Object& object)
{
    glm::vec3 rotate{0};
    if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS)
    {
        rotate.y += 1.0f;
    }
    if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS)
    {
        rotate.y -= 1.0f;
    }
    if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS)
    {
        rotate.x += 1.0f;
    }
    if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS)
    {
        rotate.x -= 1.0f;
    }


    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
    {
        object.transform.rotation += look_speed * dt * glm::normalize(rotate);
    }

    object.transform.rotation.x = glm::clamp(object.transform.rotation.x, -1.5f, 1.5f); // in radians
    object.transform.rotation.y = glm::mod(object.transform.rotation.y,  glm::two_pi<float>());

    float yaw = object.transform.rotation.y;
    const glm::vec3 forward_dir{sin(yaw), 0.0f, cos(yaw)};
    const glm::vec3 right_dir{cos(yaw), 0.0f, -sin(yaw)};
    const glm::vec3 up_dir{0.0f, -1.0f, 0.0f};

    glm::vec3 move_dir{0.0f};
    if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS)
    {
        move_dir += forward_dir;
    }
    if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS)
    {
        move_dir -= forward_dir;
    }
    if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS)
    {
        move_dir += right_dir;
    }
    if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS)
    {
        move_dir -= right_dir;
    }
    if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS)
    {
        move_dir += up_dir;
    }
    if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS)
    {
        move_dir -= up_dir;
    }

    if (glm::dot(move_dir, move_dir) > std::numeric_limits<float>::epsilon())
    {
        object.transform.translation += move_speed * dt * glm::normalize(move_dir);
    }
}


void KeyboardController::moveTowardsTarget(GLFWwindow* window, float dt, Object& object, glm::vec3 target)
{
    glm::vec3 towards_dir = target - object.transform.translation;

    std::cout << "distance to target: " << glm::length(towards_dir) << std::endl;

    if (glm::dot(towards_dir, towards_dir) < std::numeric_limits<float>::epsilon())
    {
        towards_dir = {1.0f, 0.0f, 0.0f};
    }

    int move_to = 0;
    if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS)
    {
        move_to += 1;
    }
    if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS)
    {
        move_to -= 1;
    }

    const float MIN_OBJ_DISTANCE = 0.1f;
    if (glm::dot(towards_dir, towards_dir) < MIN_OBJ_DISTANCE && move_to > 0)
    {
        return;
    }

    object.transform.translation += (move_speed * dt * move_to) * glm::normalize(towards_dir);

}

void KeyboardController::moveAroundTarget(GLFWwindow* window, float dt, Object& object, glm::vec3 target)
{
    const glm::vec3 towards_dir = target - object.transform.translation;

    if (glm::dot(towards_dir, towards_dir) < std::numeric_limits<float>::epsilon())
    {
        return;
    }

    int move_right = 0;
    int move_up = 0;

    if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS)
    {
        move_right += 1;
    }
    if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS)
    {
        move_right -= 1;
    }
    if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS)
    {
        move_up += 1;
    }
    if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS)
    {
        move_up -= 1;
    }

    if (move_up == 0 && move_right == 0)
    {
        return;
    }

    // angle between X and projection of the towards_dir vector on the XZ plane
    float horizontal_length = glm::length(glm::vec3{towards_dir.x, 0, towards_dir.z});
    float horizontal_angle = glm::atan(towards_dir.x, towards_dir.z);
    float new_horizontal_angle = horizontal_angle - rotation_speed * dt * move_right;

    // std::cout << "horizontal_angle = " << horizontal_angle << std::endl;

    glm::vec3 new_towards_dir = {horizontal_length * glm::sin(new_horizontal_angle), towards_dir.y, horizontal_length * glm::cos(new_horizontal_angle)};

    const float MAX_VERTICAL_ANGLE = 1.4f;
    const float MIN_VERTICAL_ANGLE = -1.4f;

    // angle between the towards_dir vector and the XZ plane
    float vertical_angle = glm::asin(towards_dir.y / glm::length(towards_dir));
    float new_vertical_angle = vertical_angle + rotation_speed * dt * move_up;

    std::cout << "vertical_angle = " << vertical_angle << std::endl;

    if (!(new_vertical_angle > MAX_VERTICAL_ANGLE && move_up > 0
        || new_vertical_angle < MIN_VERTICAL_ANGLE && move_up < 0))
    {
        new_towards_dir.y = glm::length(towards_dir) * glm::sin(new_vertical_angle);
    }

    object.transform.translation = target - new_towards_dir;
}

} // namespace yLab
