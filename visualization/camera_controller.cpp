#include "camera_controller.hpp"
#include <limits>
#include <iostream>

namespace yLab
{

// helper function
glm::vec3 rotationMatrixToAngles(glm::vec3 u, glm::vec3 v, glm::vec3 w)
{
    glm::vec3 rotation{};

    float r00 = u.x;
    float r10 = u.y;
    float r01 = v.x;
    float r11 = v.y;
    float r02 = w.x;
    float r12 = w.y;
    float r22 = w.z;

    if (r12 < 1)
    {
        if (r12 > -1)
        {
            rotation.x = glm::asin(-r12);
            rotation.y = glm::atan(r02, r22);
            rotation.z = glm::atan(r10, r11);
        }
        else //r12 = −1
        {
            //Not a unique solution: rotation.z − thetaY = atan2(−r01, r00)
            rotation.x = glm::half_pi<float>();
            rotation.y = -glm::atan(-r01, r00);
            rotation.z = 0;
        }
    }
    else //r12 = 1
    {
        //Not a unique solution: rotation.z + rotation.y = atan2(−r01, r00)
        rotation.x = -glm::half_pi<float>();
        rotation.y = glm::atan(-r01, r00);
        rotation.z = 0;
    }

    return rotation;
}

// helper function
auto angelsToRotationMatrix(glm::vec3 rotation)
-> std::tuple<glm::vec3, glm::vec3, glm::vec3>
{
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);

    // u, v, w are columns in rotation matrix (rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3))
    const glm::vec3 u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
    const glm::vec3 v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
    const glm::vec3 w{(c2 * s1), (-s2), (c1 * c2)};

    return {u, v, w};
}

void CameraController::moveInPlaneXZ(float dt)
{
    float local_look_speed = look_speed;
    if (glfwGetKey(window, keys.accelerate) == GLFW_PRESS)
    {
        local_look_speed *= look_acceleration_multiplier;
    }
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


    glm::vec3 rotation = rotationMatrixToAngles(camera_data.u, camera_data.v, camera_data.w);
    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
    {
        rotation += local_look_speed * dt * glm::normalize(rotate);
    }

    rotation.x = glm::clamp(rotation.x, -1.5f, 1.5f); // in radians
    rotation.y = glm::mod(rotation.y,  glm::two_pi<float>());

    auto [U, V, W] = angelsToRotationMatrix(rotation);
    camera_data.u = U;
    camera_data.v = V;
    camera_data.w = W;

    float yaw = rotation.y;
    const glm::vec3 forward_dir{sin(yaw), 0.0f, cos(yaw)};
    const glm::vec3 right_dir{cos(yaw), 0.0f, -sin(yaw)};
    const glm::vec3 up_dir{0.0f, -1.0f, 0.0f};


    float local_move_speed = move_speed;
    if (glfwGetKey(window, keys.accelerate) == GLFW_PRESS)
    {
        local_move_speed *= move_acceleration_multiplier;
    }

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
        camera_data.translation += local_move_speed * dt * glm::normalize(move_dir);
    }
}


void CameraController::moveTowardsTarget(float dt, glm::vec3 target)
{
    glm::vec3 towards_dir = target - camera_data.translation;

    // std::cout << "distance to target: " << glm::length(towards_dir) << std::endl;

    float local_move_speed = move_speed;
    if (glfwGetKey(window, keys.accelerate) == GLFW_PRESS)
    {
        local_move_speed *= move_acceleration_multiplier;
    }

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

    camera_data.translation += (local_move_speed * dt * move_to) * glm::normalize(towards_dir);
}

void CameraController::moveAroundTarget(float dt, glm::vec3 target)
{
    const glm::vec3 towards_dir = target - camera_data.translation;

    if (glm::dot(towards_dir, towards_dir) < std::numeric_limits<float>::epsilon())
    {
        return;
    }

    int move_right = 0;
    int move_up = 0;

    float local_rotation_speed = rotation_speed;
    if (glfwGetKey(window, keys.accelerate) == GLFW_PRESS)
    {
        local_rotation_speed *= rotation_acceleration_multiplier;
    }

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
    float new_horizontal_angle = horizontal_angle - local_rotation_speed * dt * move_right;

    // std::cout << "horizontal_angle = " << horizontal_angle << std::endl;

    glm::vec3 new_towards_dir = {horizontal_length * glm::sin(new_horizontal_angle), towards_dir.y, horizontal_length * glm::cos(new_horizontal_angle)};

    const float MAX_VERTICAL_ANGLE = 1.4f;
    const float MIN_VERTICAL_ANGLE = -1.4f;

    // angle between the towards_dir vector and the XZ plane
    float vertical_angle = glm::asin(towards_dir.y / glm::length(towards_dir));
    float new_vertical_angle = vertical_angle + local_rotation_speed * dt * move_up;

    // std::cout << "vertical_angle = " << vertical_angle << std::endl;

    if (!(new_vertical_angle > MAX_VERTICAL_ANGLE && move_up > 0
        || new_vertical_angle < MIN_VERTICAL_ANGLE && move_up < 0))
    {
        new_towards_dir.y = glm::length(towards_dir) * glm::sin(new_vertical_angle);
    }

    camera_data.translation = target - new_towards_dir;
}

void CameraController::lookInDirection(glm::vec3 direction, glm::vec3 up)
{
    assert(glm::dot(up, up) > std::numeric_limits<float>::epsilon());

    camera_data.w = glm::normalize(direction);
    camera_data.u = glm::normalize(glm::cross(camera_data.w, up));
    camera_data.v = glm::cross(camera_data.w, camera_data.u);
}

void CameraController::lookOnTarget(glm::vec3 target, glm::vec3 up)
{
    assert(glm::dot(up, up) > std::numeric_limits<float>::epsilon());
    lookInDirection(target - camera_data.translation, up);
}


void CameraController::swichCamMode(const std::vector<int> camera_mods)
{
    assert(camera_mods.size() == 2);
    if (glfwGetKey(window, keys.CamMode0) == GLFW_PRESS)
    {
        camera_data.movement_mode = camera_mods[0];
    }
    if (glfwGetKey(window, keys.CamMode1) == GLFW_PRESS)
    {
        camera_data.movement_mode = camera_mods[1];
    }
}

void CameraController::moveHome(glm::vec3 home_position)
{
    if (glfwGetKey(window, keys.goHome) == GLFW_PRESS)
    {
        camera_data.translation = home_position;
    }
}

} // namespace yLab
