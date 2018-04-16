#include "Transformable.hpp"

#include <glm/glm.hpp>

namespace sf3d {

/* Properties */
/// Position
void Transformable::setPosition(const glm::vec3& position) {
	_position = position;
}
const glm::vec3& Transformable::getPosition() const {
	return _position;
}

/// Rotation
void Transformable::setRotation(const glm::vec3& direction) {
	_rotation = direction;
}
const glm::vec3& Transformable::getRotation() const {
	return _rotation;
}

/// Scale
void Transformable::setScale(const glm::vec3& value) {
	_scale = value;
}
const glm::vec3& Transformable::getScale() const {
	return _scale;
}

/// Origin
void Transformable::setOrigin(const glm::vec3& position) {
	_origin.x = position.x;
	_origin.y = -position.y;
	_origin.z = position.z;
}
const glm::vec3& Transformable::getOrigin() const {
	return _origin;
}



/* Methods */
// Transform
Transform Transformable::getTransform() const {
	Transform transform;
	transform.translate(_position);
	transform.rotate(glm::radians(_rotation.x), {1.f, 0.f, 0.f});
	transform.rotate(glm::radians(_rotation.y), {0.f, 1.f, 0.f});
	transform.rotate(glm::radians(_rotation.z), {0.f, 0.f, 1.f});
	transform.translate(-(_origin*_scale));
	transform.scale(_scale);

	return transform;
}

// Position
void Transformable::translate(const glm::vec3& offset) {
	_position += offset;
}

// Rotation
void Transformable::rotate(const glm::vec3& direction) {
	_rotation += direction;
}

// Scale
void Transformable::scale(const glm::vec3& value) {
	_scale *= value;
}

}
