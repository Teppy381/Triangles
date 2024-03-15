#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace yLab
{

class Camera
{
public:
    void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
    void setPerspectiveProjection(float fov_y, float aspect, float near, float far);

    void setViewMatrix(glm::vec3 position, glm::vec3 u, glm::vec3 v, glm::vec3 w);

    const glm::mat4& getProjection() const
    {
        return projection_matrix;
    }
    const glm::mat4& getView() const
    {
        return view_matrix;
    }

private:
    glm::mat4 projection_matrix;
    glm::mat4 view_matrix{1.0f};
};

} // namespace yLab
