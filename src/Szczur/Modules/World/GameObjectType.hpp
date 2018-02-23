#pragma once

/** @file GameObjectType.hpp
 ** @description Header file with object type class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>
#include <vector>

namespace rat
{

/** @class GameObjectType
 ** @description Contains informations about types of certain in-game objects.
 **/

class GameObjectType 
{
	/* Types */
public:
	enum class DisplayType {
		None,
		Sprite,
		AnimatedSprite,
		Armature
	};
	struct SpriteDisplayData {
		sf::Texture*				texture	= nullptr; // @todo ? Maybe should it be done without external AssetManager at all?
		sf::Vector2f				origin	{0, 0};
		std::vector<sf::IntRect>	rectMap;
	};
	// struct AnimatedSpriteDisplayData {
	// };
	// struct ArmatureDisplayData {
	// };
	
	
	
	/* Fields */
public:
	// Common
	std::string 				name 			{""};
	DisplayType 				type			{DisplayType::None};

	// State strings
	std::vector<std::string>	stateStrings	{"default"};

	// Specialized
	union {
		SpriteDisplayData spriteData;
		//AnimatedSpriteDisplayData animatedSpriteData;
		//ArmatureDisplayData armatureData;
	};



	/* Operators */
public:
	// Loads the object type from configuration file.
	GameObjectType(const std::string& name);

	// @info Testing propouses
	GameObjectType(const std::string& name, SpriteDisplayData data);
	// GameObjectType(const std::string& name, AnimatedSpriteDisplayData& data);
	// GameObjectType(const std::string& name, ArmatureDisplayData& data);
};

}
