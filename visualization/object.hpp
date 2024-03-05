#pragma once

#include "model.hpp"

#include <memory>

namespace yLab{

struct Transform2dComponent {
    glm::vec2 translation{};  // (position offset)
    glm::vec2 scale{1.0f, 1.0f};
    float rotation;

    glm::mat2 mat2() {
        const float s = glm::sin(rotation);
        const float c = glm::cos(rotation);
        glm::mat2 rot_matrix{{c, s}, {-s, c}};

        glm::mat2 scale_mat{{scale.x, 0.0f}, {0.0f, scale.y}};
        return rot_matrix * scale_mat;
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

    Object(const Object &) = delete;
    Object &operator=(const Object &) = delete;
    Object(Object &&) = default;
    Object &operator=(Object &&) = default;

    id_t getId() { return id; }

    std::shared_ptr<Model> model{};
    glm::vec3 color{};
    Transform2dComponent transform2d{};

private:
    Object(id_t obj_id) : id{obj_id} {}

    id_t id;
};

} // namespace yLab
