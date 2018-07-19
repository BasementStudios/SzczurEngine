#include "BattleSprite.hpp"

#include "BattleScene.hpp"
#include "Battle.hpp"
#include "BattleTextureManager.hpp"

#include <Szczur/Modules/Script/Script.hpp>

#include <Szczur/Utility/SFML3D/Texture.hpp>
#include <Szczur/Utility/SFML3D/RenderTarget.hpp>


namespace rat
{

BattleSprite::BattleSprite(BattleScene* scene)
	: _scene(scene),
	_textureManager(detail::globalPtr<Battle>->getTextureManager())
{	
	_sprite.setPosition(_scene->getPosition());
}
	
void BattleSprite::setPosition(glm::vec2 position)
{
	_position = position;

	glm::vec3 pos = _sprite.getPosition();
	pos.x = _position.x;
	pos.z = _position.y;
	_sprite.setPosition(pos);
}

glm::vec2 BattleSprite::getPosition()
{
	return _position;
}

void BattleSprite::setOrigin(glm::vec2 origin)
{
	_sprite.setOrigin({origin.x, origin.y, _sprite.getOrigin().z});
}

glm::vec2 BattleSprite::getOrigin()
{
	return glm::vec2(_sprite.getOrigin().x, _sprite.getOrigin().y);
}

void BattleSprite::setScale(float scale)
{
	_scale = scale;
	_sprite.setScale({_scale, _scale, 1.f});
}

void BattleSprite::setHeight(float height)
{
	glm::vec3 pos = _sprite.getPosition();
	pos.y = _scene->getPosition().y + height;
	_sprite.setPosition(pos);
}

void BattleSprite::move(glm::vec2 direction)
{
	_position += direction;

	glm::vec3 pos = _sprite.getPosition();
	pos.x = _position.x;
	pos.z = _position.y;
	_sprite.setPosition(pos);
}

void BattleSprite::kill()
{
	_isLife = false;
}

bool BattleSprite::isAlive()
{
	return _isLife;
}

sf3d::Sprite& BattleSprite::getSprite()
{
	return _sprite;
}

void BattleSprite::setTexture(const std::string& textureName)
{
	_sprite.setTexture(_textureManager.getTexture(textureName));
}

void BattleSprite::render(sf3d::RenderTarget& canvas)
{
	canvas.draw(_sprite);
}

void BattleSprite::initScript(Script& script)
{
	auto object = script.newClass<BattleSprite>("BattleSprite", "Battle");

	object.set("setPosition", &BattleSprite::setPosition);
	object.set("getPosition", &BattleSprite::getPosition);
	object.set("move", &BattleSprite::move);

	object.set("setOrigin", &BattleSprite::setOrigin);
	object.set("getOrigin", &BattleSprite::getOrigin);

	object.set("setScale", &BattleSprite::setScale);

	object.set("setHeight", &BattleSprite::setHeight);
	
	object.set("kill", &BattleSprite::kill);
	object.set("isAlive", &BattleSprite::isAlive);

	object.set("setTexture", &BattleSprite::setTexture);
	object.set("getSprite", &BattleSprite::getSprite);

	object.init();
}

}