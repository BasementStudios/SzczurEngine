#pragma once

/** @file GameObject.hpp
 ** @description Header file with sprite object class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <vector>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "GameObject.hpp"

namespace rat
{

/** @class SpriteGameObject
 ** @extends GameObject
 ** @description Simplest, one sprite object.
 **/
class SpriteGameObject : public GameObject 
{	
	/* Fields */
protected:
	/// Vertices for increase performace
	sf::Vertex		vertices[4];


	
	/* Properties */
protected:
	/** @property Texture
	 ** @description Texture used to draw the object.
	 ** @access const get
	 **/
	const sf::Texture& getTexture() const;

	/** @property Origin
	 ** @description Origin on texture for the object center.
	 ** @access const get
	 **/
	const sf::Vector2f getOrigin() const;
	
	/** @property TextureRect
	 ** @description Defines part of texture used to draw the object in current state.
	 ** @access const get
	 **/
	const sf::IntRect getTextureRect() const;

public:
	/** @propertyhook setState
	 ** @description Detects object state changes.
	 **/
	virtual void setState(const StateID_t state);



	/* Operators */
public:
	SpriteGameObject(const GameObjectType* type, const std::string& name, const std::string& state = "default", const Vector_t& position = {0.f, 0.f}, const Vector_t& speed = {0.f, 0.f});
};

}
