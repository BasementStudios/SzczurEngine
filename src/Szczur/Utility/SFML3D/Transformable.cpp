#include "Transformable.hpp"

/** @file Transformable.cpp
 ** @description Implementaion file with Transformable class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 ** @author Tomasz (Knayder) Jatkowski <knayder@outlook.com>
 **/

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>	// translate, rotate, scale

namespace rat::sf3d
{

	/* Properties */
	/// Position
	const Transformable::Vector_t& Transformable::getPosition() const
	{
		return this->_position;
	}
	void Transformable::setPosition(const Vector_t& value)
	{
		this->_position = value;
	}

	/// Rotation
	const Transformable::Vector_t& Transformable::getRotation() const
	{
		return this->_position;
	}
	void Transformable::setRotation(const Vector_t& value)
	{
		this->_position = value;
	}

	/// Scale
	const Transformable::Vector_t& Transformable::getScale() const
	{
		return this->_position;
	}
	void Transformable::setScale(const Vector_t& value)
	{
		this->_position = value;
	}

	const Transformable::Vector_t& Transformable::getOrigin() const {
		return _origin;
	}

	void Transformable::setOrigin(const Vector_t& value) {
		_origin = value;
	}



	/* Operators */
	/// Constructor
	Transformable::Transformable(
		const Vector_t& position		= {0.f, 0.f, 0.f},
			const Vector_t& rotation	= {0.f, 0.f, 0.f},
			const Vector_t& scale		= {1.f, 1.f, 1.f},
			const Vector_t& origin		= {0.f, 0.f, 0.f}
	)
		: _position(position), _rotation(rotation), _scale(scale), _origin(origin)
	{
		;
	}



	/* Methods */
	/// compute
	Transform Transformable::getTransform() const
	{
		Transform transform;

		transform.translate(-_origin);
		transform.rotate(_rotation.x, {1.f, 0.f, 0.f});
		transform.rotate(_rotation.y, {0.f, 1.f, 0.f});
		transform.rotate(_rotation.z, {0.f, 0.f, 1.f});
		transform.translate(_position);
		transform.scale(_scale);

		return transform;
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