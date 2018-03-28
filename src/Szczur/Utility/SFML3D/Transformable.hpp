#pragma once

/** @file Transformable.hpp
 ** @description Header file with Transformable class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 ** @author Tomasz (Knayder) Jatkowski <knayder@outlook.com>
 **/

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace rat::sf3d
{

	/** @class Transformable
	 ** @description Class for game world elements containing position, rotation and scale.
	**/
	class Transformable
	{
		/* Types */
	public:
		using Vector_t = glm::vec3;
		using Matrix_t = glm::mat4;



		/* Variables */
	public:
		Vector_t _position	{};
		Vector_t _rotation	{};
		Vector_t _scale		{1.f, 1.f, 1.f};
		Vector_t _origin	{};



		/* Properties */
	public:
		/** @property Position
		 ** @description Position in the game world.
		** @access get set
		**/
		const Vector_t& getPosition() const;
		void setPosition(const Vector_t& value);
		
		/** @property Rotation
		 ** @description Rotation of the element in the game world by angles for each axis as clockwork.
		** @access get set
		**/
		const Vector_t& getRotation() const;
		void setRotation(const Vector_t& value);
		
		/** @property Scale
		 ** @description Scale of the element in the game world for each axis.
		** @access get set
		**/
		const Vector_t& getScale() const;
		void setScale(const Vector_t& value);


		const Vector_t& getOrigin() const;
		void setOrigin(const Vector_t& value);



		/* Operators */
	public:
		/// Constructor
		Transformable(
			const Vector_t& position	= {0.f, 0.f, 0.f},
			const Vector_t& rotation	= {0.f, 0.f, 0.f},
			const Vector_t& scale		= {1.f, 1.f, 1.f},
			const Vector_t& origin		= {0.f, 0.f, 0.f}
		);


		
		/* Methods */
	public:
		/** @method compute
		 ** @description Returns transform matrix as result of current Transformable state.
		**/
		Transform getTransform() const;

		void translate(const Vector_t& offset);

		void rotate(const Vector_t& offset);

		void scale(const Vector_t& offset);
	};

}
