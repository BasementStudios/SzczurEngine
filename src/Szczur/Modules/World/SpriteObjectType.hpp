#pragma once

/** @file SpriteObjectType.hpp
 ** @description Header file with Sprite Object Type class as shared game sprite object type information.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>
#include <array>
#include <vector>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include "Szczur/Modules/World/Object.hpp"
#include "Szczur/Modules/World/ObjectType.hpp"

namespace rat
{

class SpriteObjectType : public ObjectType
{
	/* Types */
protected:
	struct StateDetails
	{
		std::string 				name;
		Object::Vector_t 			origin;
		std::array<sf::Vertex, 4>	vertices;
	};

	using StateDetails_t 			= StateDetails;
	template <typename T>
	using StatesDetailsContainer_t 	= std::vector<T>;

public:
	using StateID_t = StatesDetailsContainer_t<StateDetails_t>::size_type;
	
	
	
	/* Constants */
public:
	constexpr static StateID_t  defaultStateID 		{0};
	constexpr static auto       defaultStateString  {"default"};
protected:
	constexpr static auto	spritesAssetsPath	= "Assets/Objects/";
	constexpr static auto	configFilePath		= "/sprite.json";
	constexpr static auto 	textureFilePath		= "/texture.png";
	

	
	/* Fields */
protected:
	sf::Texture 								texture;
	StatesDetailsContainer_t<StateDetails_t>	statesDetails;



	/* Properties */
public:
	/** @property Texture
	 ** @description Texture used to draw the object.
	 ** @access const get
	 **/
	const sf::Texture& getTexture() const;

	/** @property State
	 ** @description State string/ID used to identify states of Sprite.
	 ** @acess const get
	 **/
	const std::string& getStateString(const StateID_t& stateID) const;
	StateID_t getStateID(const std::string& stateString) const;

	/** @property Origin
	 ** @description Origin on texture for the object center.
	 ** @access const get
	 ** @argument stateID - since returns for specific state
	 **/
	const Object::Vector_t getOrigin(const StateID_t& stateID) const;

	/** @property Vertices
	 ** @description Vertices used to render the texture as the object.
	 ** @access const get
	 ** @argument stateID - since returns for specific state
	 **/
	const std::array<sf::Vertex, 4> getVertices(const StateID_t& stateID) const;

	

	/* Operators */
public:
	/** @constructor
	 ** @description Loads sprite object type.
	 ** @argument name - name of the sprite object type to load.
	 **/
	SpriteObjectType(const std::string& name);

	// Disable coping
	SpriteObjectType(const SpriteObjectType&) = delete;
	SpriteObjectType& operator = (const SpriteObjectType&) = delete;

	// Disable moving
	SpriteObjectType(SpriteObjectType&&) = delete;
	SpriteObjectType& operator = (SpriteObjectType&&) = delete;
	



	/* Functions */
protected:
	std::array<sf::Vertex, 4> calculateVertices(const sf::IntRect& rect);
};

}
