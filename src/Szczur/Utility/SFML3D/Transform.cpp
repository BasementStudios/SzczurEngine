#include "Transform.hpp"

namespace sf3d {
	Transform::Transform() :
		_mat(1.f) {

	}

	void Transform::translate(float x, float y, float z) {
		translate({x,y,z});
	}

	void Transform::translate(const glm::vec3 & offset) {
		_mat = glm::translate(_mat, offset);
	}

	void Transform::rotate(float angle, float x, float y, float z) {
		rotate(angle, {x,y,z});
	}

	void Transform::rotate(float angle, const glm::vec3 & direction) {
		_mat = glm::rotate(_mat, angle, direction);
	}

	void Transform::scale(float x, float y, float z) {
		scale({x,y,z});
	}

	void Transform::scale(const glm::vec3 & value) {
		_mat = glm::scale(_mat, value);
	}

	void Transform::combine(const Transform & trans) {
		_mat *= trans._mat;
	}

	void Transform::operator*=(const Transform & trans) {
		combine(trans);
	}

	void Transform::operator*=(const glm::vec3 & value) {
		_mat *= glm::vec4(value, 1.f);
	}

	glm::mat4 & Transform::getMatrix() {
		return _mat;
	}
}
