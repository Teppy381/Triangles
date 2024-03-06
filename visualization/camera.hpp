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

    const glm::mat4& getProjection() const
    {
        return projection_matrix;
    }

private:
    glm::mat4 projection_matrix;
};

} // namespace yLab
