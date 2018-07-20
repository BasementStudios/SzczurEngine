#include "BattleAnimationSprite.hpp"

#include "BattleScene.hpp"
#include "Battle.hpp"
#include "BattleTextureManager.hpp"

#include <Szczur/Modules/Script/Script.hpp>

#include <Szczur/Utility/SFML3D/Texture.hpp>
#include <Szczur/Utility/SFML3D/RenderTarget.hpp>

namespace rat
{

BattleAnimationSprite::BattleAnimationSprite(BattleScene* scene)
	: BattleSprite(scene)
{
	restart();
}

void BattleAnimationSprite::setTimestamp(float value)
{
	_timestamp = value;
}

void BattleAnimationSprite::setFrames(glm::vec2 frameSize, int framesInRow, int framesAmount)
{
	_frameSize = frameSize;
	_framesInRow = framesInRow;
	_framesAmount = framesAmount;
}

void BattleAnimationSprite::setFrame(int index)
{
	if(index >= _framesAmount)
		index = _framesAmount-1;

	if(index < 0)
		index = 0;

	_currentFrame = index;
}

void BattleAnimationSprite::setFrameSize(glm::vec2 frameSize)
{
	_frameSize = frameSize;
}

void BattleAnimationSprite::setFramesInRow(int framesInRow)
{
	_framesInRow = framesInRow;
}

void BattleAnimationSprite::setFramesAmount(int framesAmount)
{
	_framesAmount = framesAmount;
}

void BattleAnimationSprite::stopAtLastFrame(bool flag)
{
	_stopAtLastFrame = flag;
}

void BattleAnimationSprite::removeOnLastFrame(bool flag)
{
	_removeOnLastFrame = flag;
}

void BattleAnimationSprite::restart()
{
	_clock.restart();
	_currentFrame = 0;
	_lastTime = 0;
}

void BattleAnimationSprite::render(sf3d::RenderTarget& canvas)
{
	float elapsed = _clock.getElapsedTime().asSeconds() + _lastTime;
	_lastTime = std::fmod(elapsed, _timestamp);
	_currentFrame += std::floor(elapsed/_timestamp);
	if(_currentFrame >= _framesAmount) {
		if(_stopAtLastFrame) {
			_currentFrame = _framesAmount-1;
		}
		else {
			_currentFrame %= _framesAmount;
		}
		if(_removeOnLastFrame) {
			kill();
		}
	}
	_updateSprite();

	canvas.draw(_sprite);

	_clock.restart();
}

void BattleAnimationSprite::initScript(Script& script)
{
	auto object = script.newClass<BattleAnimationSprite>("BattleAnimationSprite", "Battle");

// Sprite

	object.set("setPosition", &BattleAnimationSprite::setPosition);
	object.set("getPosition", &BattleAnimationSprite::getPosition);
	object.set("move", &BattleAnimationSprite::move);

	object.set("setOrigin", &BattleAnimationSprite::setOrigin);
	object.set("getOrigin", &BattleAnimationSprite::getOrigin);

	object.set("setScale", &BattleAnimationSprite::setScale);

	object.set("setHeight", &BattleAnimationSprite::setHeight);
	
	object.set("kill", &BattleAnimationSprite::kill);

	object.set("setTexture", &BattleAnimationSprite::setTexture);
	object.set("getSprite", &BattleAnimationSprite::getSprite);

// Animation sprite

	object.set("setTimestamp", &BattleAnimationSprite::setTimestamp);

	object.set("setFrames", &BattleAnimationSprite::setFrames);
	object.set("setFrameSize", &BattleAnimationSprite::setFrameSize);
	object.set("setFramesInRow", &BattleAnimationSprite::setFramesInRow);
	object.set("setFramesAmount", &BattleAnimationSprite::setFramesAmount);

	object.set("setFrame", &BattleAnimationSprite::setFrame);

	object.set("stopAtLastFrame", &BattleAnimationSprite::stopAtLastFrame);
	object.set("removeOnLastFrame", &BattleAnimationSprite::removeOnLastFrame);

	object.set("restart", &BattleAnimationSprite::restart);

	object.init();
}

void BattleAnimationSprite::_updateSprite()
{
	glm::ivec2 index(
		_currentFrame % _framesInRow,
		_currentFrame / _framesInRow
	);

	_sprite.setTextureRect(_frameSize*index, _frameSize);
}

}