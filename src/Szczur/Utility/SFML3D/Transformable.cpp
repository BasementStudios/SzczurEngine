#include "Transformable.hpp"
#include <glm/glm.hpp>
#include <iostream>
namespace sf3d {
	void Transformable::setPosition(const glm::vec3 & position) {
		_position = position;
	}

	void Transformable::move(const glm::vec3 & offset) {
		_position += offset;
	}

	const glm::vec3 & Transformable::getPosition() const {
		return _position;
	}

	void Transformable::setRotation(const glm::vec3 & direction) {
		_rotation = direction;
	}

	void Transformable::rotate(const glm::vec3 & direction) {
		_rotation += direction;
	}

	const glm::vec3 & Transformable::getRotation() const {
		return _rotation;
	}

	void Transformable::setScale(const glm::vec3 & value) {
		_scale = value;
	}

	void Transformable::scale(const glm::vec3 & value) {
		_scale *= value;
	}

	const glm::vec3 & Transformable::getScale() const {
		return _scale;
	}

	void Transformable::setOrigin(const glm::vec3 & position) {
		_origin.x = position.x;
		_origin.y = -position.y;
		_origin.z = position.z;
	}

	const glm::vec3 & Transformable::getOrigin() const {
		return _origin;
	}

	Transform Transformable::getTransform() const {
		Transform transform;
		transform.rotate(glm::radians(_rotation.x), {1.f, 0.f, 0.f});
		transform.rotate(glm::radians(_rotation.y), {0.f, 1.f, 0.f});
		transform.rotate(glm::radians(_rotation.z), {0.f, 0.f, 1.f});
		transform.translate(_position - _origin);
		transform.scale(_scale);

		return transform;
	}
}

