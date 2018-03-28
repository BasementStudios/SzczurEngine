#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace rat::sf3d {
    class Transform {
    public:
        Transform();

        void translate(const glm::vec3& offset);

        void rotate(float angle, const glm::vec3& direction);

        void scale(const glm::vec3& offset);

        void combine(const Transform& transform);

        operator glm::mat4();

        void operator*=(const Transform& object);

    //private:
        glm::mat4 _mat;
    };
}