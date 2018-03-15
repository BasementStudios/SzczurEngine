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
	SpriteObjectType::StateID_t stateID {SpriteObjectType::defaultStateID};



	/* Properties */
public:
	/** @property SpriteType
     ** @description Class/type of the object.
	 ** @access const get
	 **/
    const SpriteObjectType* getSpriteType() const;

public:
	/** @property State
     ** @description The in-game object state (ID).
     ** @access get set & string get/set
     ** @virtual set    
     **/
    const SpriteObjectType::StateID_t getState() const;
    void setState(const SpriteObjectType::StateID_t& stateID);
    const std::string& getStateString() const;
    void setState(const std::string& stateString);

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
	SpriteObject(
		const SpriteObjectType* 			type, 
		const std::string& 					name, 
		const Object::Vector_t& 			position 	= {0.f, 0.f}, 
		const Object::Vector_t& 			speed 		= {0.f, 0.f},
		const SpriteObjectType::StateID_t& 	stateID 	= SpriteObjectType::defaultStateID
	);



	/* Methods */
public:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
};

}
