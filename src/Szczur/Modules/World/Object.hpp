#pragma once

/** @file Object.hpp
 ** @description Header file with base game object class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "ObjectType.hpp"

namespace rat
{

/** @class Object
 ** @inherit sf::Drawable
 ** @description Base class for in-game objects.
 **/
class Object : public sf::Drawable
{   
    /* Types */
public:
	using Vector_t = sf::Vector2f;
	using StateID_t = ObjectType::StateID_t;



	/* Variables */
public:
	std::string	name;
protected:
	ObjectType*	type;
	StateID_t	stateID		{ObjectType::defaultStateID};

	Vector_t	position	{0.f, 0.f};
	Vector_t	speed		{0.f, 0.f};



	/* Properties */
public:
    /** @property Type
     ** @description Class/type of the object.
	 ** @access get & string get
	 **/
    const ObjectType* 	getType() const;
	const std::string& 	getTypeString() const;
    
    /** @property State
     ** @description The in-game object state (ID).
     ** @access get set & string get/set
     ** @virtual set    
     ** @info virtual setter for detecting updates
     **/
    const StateID_t getState() const;
    virtual void    setState(const StateID_t stateID);
    const std::string& getStateString() const;
    void               setState(const std::string& stateString);
    
    /** @property Position
     ** @description Position on the level/map.
     ** @access get set
     **/
    Vector_t getPosition() const;
    void     setPosition(const Vector_t& position);
    
    /** @property Speed
     ** @description Current speed of the object, as delta per second.
     ** @access get set
     **/
    Vector_t getSpeed() const;
    void     setSpeed(const Vector_t& speed);



    /* Operators */
protected:
	/** @constructor Full
	 ** @note Non-public to avoid use of base class.
	 **/
    Object(ObjectType* type, const std::string& name, StateID_t stateID = ObjectType::defaultStateID, Vector_t position = {0.f, 0.f}, Vector_t speed = {0.f, 0.f});



    /* Methods */
public:
    /// Updates object logic
    virtual void update(float deltaTime);

    /// Draws the object
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override = 0;
};

}
