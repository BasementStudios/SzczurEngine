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
public:
	using StateID_t = std::size_t;
protected:
	struct StateDetails
	{
		std::string 				name;
		Object::Vector_t 			origin;
		std::array<sf::Vertex, 4>	vertices;
	};
	
	
	
	/* Constants */
public:
	constexpr static StateID_t  defaultStateID 		{0};
	constexpr static auto       defaultStateString  {"default"};
protected:
	constexpr static auto	assetsPath		= "Assets/Objects/";
	constexpr static auto	configFileName	= "object.json";
	constexpr static auto 	textureFileName	= "texture.png";
	

	
	/* Fields */
protected:
	sf::Texture 				texture;
	std::vector<StateDetails> 	statesDetails;



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
	SpriteObjectType::StateID_t getStateID(const std::string& stateString) const;
	const std::string& getStateString(SpriteObjectType::StateID_t stateID) const;

	/** @property Origin
	 ** @description Origin on texture for the object center.
	 ** @access const get
	 ** @argument stateID - since returns for specific state
	 **/
	const Object::Vector_t getOrigin(SpriteObjectType::StateID_t stateID) const;

	/** @property Vertices
	 ** @description Vertices used to render the texture as the object.
	 ** @access const get
	 ** @argument stateID - since returns for specific state
	 **/
	const std::array<sf::Vertex, 4> getVertices(SpriteObjectType::StateID_t stateID) const;

	

	/* Operators */
public:
	/** @constructor
	 ** @description Loads sprite object type.
	 ** @argument name - name of the sprite object type to load.
	 **/
	SpriteObjectType(const std::string& name);



	/* Functions */
protected:
	std::array<sf::Vertex, 4> calculateVertices(const sf::IntRect& rect);
};

}
