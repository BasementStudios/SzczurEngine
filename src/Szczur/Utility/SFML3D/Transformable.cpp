#include "Transformable.hpp"

#include <glm/vec3.hpp>
#include <glm/trigonometric.hpp> // radians

#include "Transform.hpp"

namespace sf3d
{

/* Properties */
// Position
void Transformable::setPosition(const glm::vec3& position)
{
	_position = position;
}
const glm::vec3& Transformable::getPosition() const
{
	return _position;
}

// Rotation
void Transformable::setRotation(const glm::vec3& direction)
{
	_rotation = direction;
}
const glm::vec3& Transformable::getRotation() const
{
	return _rotation;
}

// Scale
void Transformable::setScale(const glm::vec3& value)
{
	_scale = value;
}
const glm::vec3& Transformable::getScale() const
{
	return _scale;
}

// Origin
void Transformable::setOrigin(const glm::vec3& position)
{
	_origin.x = position.x;
	_origin.y = -position.y; // @warn To chyba nie powinno być na tym etapie, bo set(x) =/= x=gett()...
	_origin.z = position.z;
}
const glm::vec3& Transformable::getOrigin() const
{
	return _origin;
}



/* Operators */
Transformable::Transformable(
	const glm::vec3& position,
	const glm::vec3& rotation,
	const glm::vec3& scale,
	const glm::vec3& origin
)
:	_position(position),
	_rotation(rotation),
	_scale(scale),
	_origin(origin)
{
	;
}



/* Methods */
void Transformable::move(const glm::vec3& offset)
{
	_position += offset;
}


void Transformable::rotate(const glm::vec3& direction)
{
	_rotation += direction;
}


void Transformable::scale(const glm::vec3& value)
{
	_scale *= value;
}

Transform Transformable::getTransform() const
{
	Transform transform;
	transform.translate(_position);
	transform.rotate(glm::radians(_rotation.x), {1.f, 0.f, 0.f});
	transform.rotate(glm::radians(_rotation.y), {0.f, 1.f, 0.f});
	transform.rotate(glm::radians(_rotation.z), {0.f, 0.f, 1.f});
	transform.translate(-(_origin*_scale));
	transform.scale(_scale);

	return transform;
}

}
