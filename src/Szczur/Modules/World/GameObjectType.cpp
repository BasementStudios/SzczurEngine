#include "GameObjectType.hpp"

/** @file GameObjectType.cpp
 ** @description Implementation file with object type class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

namespace rat
{


GameObjectType::GameObjectType(const std::string& name)
	: name(name)
{
	// @todo . loading ObjectType from `Objects/*/object.json` 

	//this->_texture = &(assets.load<sf::Texture>("Assets/Objects/" + object.getType() + "/" + object.getState() + ".png"));
	//this->setTextureRect(sf::IntRect(0, 0, _textureAsset.getRef().getSize().x, _textureAsset.getRef().getSize().y));
	//this->setTextureRect(sf::IntRect(0, 0, _texture->getSize().x, _texture->getSize().y));
}

// @info Testing propouses 
GameObjectType::GameObjectType(const std::string& name, SpriteGameObject::DisplayData data)
	: name(name), type(GameObjectType::DisplayType::Sprite), spriteData(data)
{
	;
}
// GameObjectType::GameObjectType(const std::string& name, AnimatedSpriteGameObject::DisplayData data)
// 	: name(name), type(GameObjectType::DisplayType::AnimatedSprite), animatedSpriteData(data)
// {
// 	;
// }
// GameObjectType::GameObjectType(const std::string& name, ArmatureGameObject::DisplayData data)
// 	: name(name), type(GameObjectType::DisplayType::Armature), armatureData(data)
// {
// 	;
// }

}
