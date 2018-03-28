#include "Transform.hpp"

namespace rat::sf3d {
    Transform::Transform():
    _mat(1.f) {

    }

    void Transform::translate(const glm::vec3& offset) {
        _mat = glm::translate(_mat, offset);
    }

    void Transform::rotate(float angle, const glm::vec3& direction) {
        _mat = glm::rotate(_mat, angle, direction);
    }

    void Transform::scale(const glm::vec3& offset) {
        _mat = glm::scale(_mat, offset);
    }

    void Transform::combine(const Transform& transform) {
        _mat *= transform;
    }
}