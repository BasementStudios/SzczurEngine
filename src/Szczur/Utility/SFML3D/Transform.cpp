#include "Transform.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp> // radians
#include <glm/gtc/matrix_transform.hpp>

#include "Transformable.hpp"

namespace sf3d
{

// @todo , getTransformable - it's possible, but not that efficient...

/* Properties */
glm::mat4& Transform::getMatrix()
{
	return this->matrix;
}
const glm::mat4& Transform::getMatrix() const
{
	return this->matrix;
}



/* Operators */
Transform::Transform()
:	matrix(1.f) 
{
	;
}
Transform::Transform(const glm::mat4& matrix)
:	matrix(matrix)
{
	;
}
Transform::Transform(const Transformable& transformable)
{
	this->matrix = glm::translate(glm::mat4{1.0f}, transformable.position);
	this->matrix = glm::translate(glm::mat4{1.0f}, transformable.position);
	this->matrix = glm::rotate(this->matrix, glm::radians(transformable.rotation.x), glm::vec3{1.0f, 0.0f, 0.0f});
	this->matrix = glm::rotate(this->matrix, glm::radians(transformable.rotation.y), glm::vec3{0.0f, 1.0f, 0.0f});
	this->matrix = glm::rotate(this->matrix, glm::radians(transformable.rotation.z), glm::vec3{0.0f, 0.0f, 1.0f});
	this->matrix = glm::scale(this->matrix, transformable.scalar);
	this->matrix = glm::translate(this->matrix, -transformable.origin);
}

void Transform::operator *= (const Transform& other)
{
	this->combine(other);
}
void Transform::operator *= (const glm::vec3 value)
{
	this->matrix *= glm::vec4(value, 1.f);
}



/* Methods */
void Transform::translate(const glm::vec3 offset)
{
	this->matrix = glm::translate(this->matrix, offset);
}
void Transform::translate(const float x, const float y, const float z)
{
	this->translate({x, y, z});
}

void Transform::rotate(const float angle, const glm::vec3 direction)
{
	this->matrix = glm::rotate(this->matrix, angle, direction);
}
void Transform::rotate(const float angle, const float x, const float y, const float z) {
	this->rotate(angle, {x, y, z});
}
void Transform::rotate(const glm::vec3 rotation) {
	this->matrix = glm::rotate(this->matrix, rotation.x, {1.f, 0.f, 0.f});
	this->matrix = glm::rotate(this->matrix, rotation.y, {0.f, 1.f, 0.f});
	this->matrix = glm::rotate(this->matrix, rotation.z, {0.f, 0.f, 1.f});
}
void Transform::rotate(const float x, const float y, const float z) {
	this->rotate({x, y, z});
}


void Transform::scale(const glm::vec3 value) {
	this->matrix = glm::scale(this->matrix, value);
}
void Transform::scale(const float x, const float y, const float z) {
	this->scale({x, y, z});
}

void Transform::combine(const Transform& other)
{
	this->matrix *= other.matrix;
}

}
