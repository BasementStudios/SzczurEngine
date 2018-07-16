#include "Transform.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace sf3d {
	Transform::Transform() :
		_mat(1.f) 
	{}

	void Transform::translate(const glm::vec3& offset) {
		_mat = glm::translate(_mat, offset);
	}
	void Transform::translate(const float& x, const float& y, const float& z) {
		translate({x, y, z});
	}

	void Transform::rotate(const float& angle, const glm::vec3& direction) {
		_mat = glm::rotate(_mat, angle, direction);
	}
	void Transform::rotate(const float& angle, const float& x, const float& y, const float& z) {
		rotate(angle, {x, y, z});
	}
	void Transform::rotate(const glm::vec3& rotation) {
		_mat = glm::rotate(_mat, rotation.x, {1.f, 0.f, 0.f});
		_mat = glm::rotate(_mat, rotation.y, {0.f, 1.f, 0.f});
		_mat = glm::rotate(_mat, rotation.z, {0.f, 0.f, 1.f});
	}
	void Transform::rotate(const float& x, const float& y, const float& z) {
		rotate({x, y, z});
	}


	void Transform::scale(const glm::vec3& value) {
		_mat = glm::scale(_mat, value);
	}
	void Transform::scale(const float& x, const float& y, const float& z) {
		scale({x, y, z});
	}

	void Transform::combine(const Transform& trans) {
		_mat *= trans._mat;
	}
	
	void Transform::operator *= (const Transform& trans) {
		combine(trans);
	}

	void Transform::operator *= (const glm::vec3& value) {
		_mat *= glm::vec4(value, 1.f);
	}

	glm::mat4 & Transform::getMatrix() {
		return _mat;
	}

	const glm::mat4 & Transform::getMatrix() const {
		return _mat;
	}
}
