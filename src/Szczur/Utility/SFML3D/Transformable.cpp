#include "Transformable.hpp"

/** @file Transformable.cpp
 ** @description Implementaion file with Transformable class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 ** @author Tomasz (Knayder) Jatkowski <knayder@outlook.com>
 **/

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <glm/gtc/matrix_transform.hpp>	// translate, rotate, scale

namespace rat
{

/* Properties */
/// Position
const Transformable::Vector_t& Transformable::getPosition() const
{
	return this->_position;
}
void Transformable::setPosition(const Transformable::Vector_t& value)
{
	this->_position = value;
}

/// Rotation
const Transformable::Vector_t& Transformable::getRotation() const
{
	return this->_position;
}
void Transformable::setRotation(const Transformable::Vector_t& value)
{
	this->_position = value;
}

/// Scale
const Transformable::Vector_t& Transformable::getScale() const
{
	return this->_position;
}
void Transformable::setScale(const Transformable::Vector_t& value)
{
	this->_position = value;
}



/* Operators */
/// Constructor
Transformable::Transformable(
	const Transformable::Vector_t& position,
	const Transformable::Vector_t& rotation,
	const Transformable::Vector_t& scale
)
	: _position(position), _rotation(rotation), _scale(scale)
{
	;
}



/* Methods */
/// compute
Transformable::Matrix_t Transformable::compute() const
{
	// Get identity matrix
	glm::mat4 matrix(1.f);
	
	// Apply position translation
	matrix = glm::translate(matrix, this->_position);
	
	// Apply rotation for each axis
	matrix = glm::rotate(matrix, this->_rotation.x, glm::vec3(1.f, 0.f, 0.f));
	matrix = glm::rotate(matrix, this->_rotation.y, glm::vec3(0.f, 1.f, 0.f));
	matrix = glm::rotate(matrix, this->_rotation.z, glm::vec3(0.f, 0.f, 1.f));
	
	// Apply size scaling
	matrix = glm::scale(matrix, this->_scale);

	return matrix;
}

/// translate
void Transformable::translate(const Transformable::Vector_t& offset) {
	this->_position += offset;
}

/// rotate
void Transformable::rotate(const Transformable::Vector_t& offset) {
	this->_rotation += offset;
}

/// scale
void Transformable::scale(const Transformable::Vector_t& offset) {
	this->_scale *= offset;
}

}