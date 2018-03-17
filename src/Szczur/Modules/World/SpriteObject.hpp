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

#include "Szczur/Modules/World/Object.hpp"
#include "Szczur/Modules/World/ObjectType.hpp"
#include "SpriteObjectType.hpp"

namespace rat
{

/** @class SpriteObject
 ** @implements Object
 ** @description Simplest, sprite object.
 **/
class SpriteObject : public Object 
{
	/* Fields */
protected:
	SpriteObjectType::StateID_t currentStateID {SpriteObjectType::defaultStateID};



	/* Properties */
public:
	/** @property SpriteType
     ** @description Class/type of the object.
	 ** @access const get
	 **/
    const SpriteObjectType* getSpriteType() const;

	/** @property State
     ** @description The in-game object state (ID).
     ** @access get set & string get set
     **/
    const SpriteObjectType::StateID_t getCurrentState() const;
    void setCurrentState(const SpriteObjectType::StateID_t& stateID);
    void setCurrentState(const std::string& stateString);
    const std::string& getCurrentStateString() const;

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
	const Object::Vector_t getOrigin() const;
	
	/** @property Vertices
	 ** @description Vertices used to draw the texture on the target.
	 ** @access const get
	 **/
	const std::array<sf::Vertex, 4> getVertices() const;



	/* Operators */
public:
	/// Full constructor
	SpriteObject(
		const SpriteObjectType* 			type, 
		const std::string& 					name, 
		const Object::Vector_t& 			position 	= {}, 
		const Object::Vector_t& 			speed 		= {},
		const SpriteObjectType::StateID_t& 	stateID 	= SpriteObjectType::defaultStateID
	);
	/// Standard constructor
	SpriteObject(
		const SpriteObjectType* 			type, 
		const std::string& 					name, 
		const Object::Vector_t& 			position,
		const std::string& 					stateString,
		const Object::Vector_t& 			speed			= {}
	);



	/* Methods */
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
};

}
