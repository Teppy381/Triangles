#pragma once

#include "model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace yLab
{

struct TransformComponent
{
    glm::vec3 translation{0.0f, 0.0f, 0.0f}; // (position offset)
    glm::vec3 scale{1.0f, 1.0f, 1.0f};
    glm::vec3 rotation{0.0f, 0.0f, 0.0f};

    // 3d + homogeneous coordinate for translation
    // Matrix corresponds to Translate * Ry * Rx * Rz * Scale
    // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
    glm::mat4 mat4()
    {
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        return glm::mat4
        {
            {
                scale.x * (c1 * c3 + s1 * s2 * s3),
                scale.x * (c2 * s3),
                scale.x * (c1 * s2 * s3 - c3 * s1),
                0.0f,
            },
            {
                scale.y * (c3 * s1 * s2 - c1 * s3),
                scale.y * (c2 * c3),
                scale.y * (c1 * c3 * s2 + s1 * s3),
                0.0f,
            },
            {
                scale.z * (c2 * s1),
                scale.z * (-s2),
                scale.z * (c1 * c2),
                0.0f,
            },
            {translation.x, translation.y, translation.z, 1.0f}
        };
    }
};

class Object
{
public:
    using id_t = unsigned int;

    static Object createObject()
    {
        static id_t current_id = 0;
        return Object{current_id++};
    }

    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;
    Object(Object&&) = default;
    Object& operator=(Object&&) = default;

    id_t getId()
    {
        return id;
    }

    std::shared_ptr<Model> model{};
    glm::vec3 color{};
    TransformComponent transform{};

private:
    Object(id_t obj_id) : id{obj_id}
    {
    }

    id_t id;
};

} // namespace yLab
