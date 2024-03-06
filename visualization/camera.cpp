#include "camera.hpp"

#include <cassert>
#include <limits>

namespace yLab
{
void Camera::setOrthographicProjection(
    float left, float right, float top, float bottom, float near, float far
)
{
    projection_matrix = glm::mat4{1.0f};
    projection_matrix[0][0] = 2.0f / (right - left);
    projection_matrix[1][1] = 2.0f / (bottom - top);
    projection_matrix[2][2] = 1.0f / (far - near);
    projection_matrix[3][0] = -(right + left) / (right - left);
    projection_matrix[3][1] = -(bottom + top) / (bottom - top);
    projection_matrix[3][2] = -near / (far - near);
}

void Camera::setPerspectiveProjection(float fov_y, float aspect, float near, float far)
{
    assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
    const float tan_half_fov_y = tan(fov_y / 2.0f);
    projection_matrix = glm::mat4{0.0f};
    projection_matrix[0][0] = 1.0f / (aspect * tan_half_fov_y);
    projection_matrix[1][1] = 1.0f / (tan_half_fov_y);
    projection_matrix[2][2] = far / (far - near);
    projection_matrix[2][3] = 1.0f;
    projection_matrix[3][2] = -(far * near) / (far - near);
}

void Camera::setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{
    assert(up.length() > std::numeric_limits<float>::epsilon());
    const glm::vec3 w = glm::normalize(direction);
    const glm::vec3 u = glm::normalize(glm::cross(w, up));
    const glm::vec3 v = glm::cross(w, u);

    view_matrix = glm::mat4{1.0f};
    view_matrix[0][0] = u.x;
    view_matrix[1][0] = u.y;
    view_matrix[2][0] = u.z;
    view_matrix[0][1] = v.x;
    view_matrix[1][1] = v.y;
    view_matrix[2][1] = v.z;
    view_matrix[0][2] = w.x;
    view_matrix[1][2] = w.y;
    view_matrix[2][2] = w.z;
    view_matrix[3][0] = -glm::dot(u, position);
    view_matrix[3][1] = -glm::dot(v, position);
    view_matrix[3][2] = -glm::dot(w, position);
}

void Camera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
    assert(up.length() > std::numeric_limits<float>::epsilon());
    setViewDirection(position, target - position, up);
}

void Camera::setViewYXZ(glm::vec3 position, glm::vec3 rotation)
{
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
    const glm::vec3 u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
    const glm::vec3 v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
    const glm::vec3 w{(c2 * s1), (-s2), (c1 * c2)};
    view_matrix = glm::mat4{1.0f};
    view_matrix[0][0] = u.x;
    view_matrix[1][0] = u.y;
    view_matrix[2][0] = u.z;
    view_matrix[0][1] = v.x;
    view_matrix[1][1] = v.y;
    view_matrix[2][1] = v.z;
    view_matrix[0][2] = w.x;
    view_matrix[1][2] = w.y;
    view_matrix[2][2] = w.z;
    view_matrix[3][0] = -glm::dot(u, position);
    view_matrix[3][1] = -glm::dot(v, position);
    view_matrix[3][2] = -glm::dot(w, position);
}

} // namespace yLab