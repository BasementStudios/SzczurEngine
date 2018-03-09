#pragma once

/** @file SpriteObject.hpp
 ** @description Header file with sprite object class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <array>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "Object.hpp"
#include "SpriteObjectType.hpp"

namespace rat
{

/** @class SpriteObject
 ** @extends Object
 ** @description Simplest, sprite object.
 **/
class SpriteObject : public Object 
{
	/* Properties */
	/** @property Texture
	 ** @description Texture used to draw the object.
	 ** @access const get
	 **/
	const sf::Texture& getTexture() const;

	/** @property Origin
	 ** @description Origin on texture for the object center.
	 ** @access const get
	 **/
	const Vector_t getOrigin() const;
	
	/** @property Vertices
	 ** @description Vertices used to draw the texture on the target.
	 ** @access const get
	 **/
	const std::array<sf::Vertex, 4> getVertices() const;



	/* Operators */
	SpriteObject(const SpriteObjectType* type, const std::string& name, ObjectType::StateID_t stateID = ObjectType::defaultStateID, Vector_t position = {0.f, 0.f}, Vector_t speed = {0.f, 0.f});



	/* Methods */
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
};

}
