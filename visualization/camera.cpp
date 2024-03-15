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


void Camera::setViewMatrix(glm::vec3 position, glm::vec3 u, glm::vec3 v, glm::vec3 w)
{
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
