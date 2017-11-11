#pragma once

#include <memory>

#include <dragonBones\DragonBonesHeaders.h>

#include <SFML\Graphics\Sprite.hpp>

class WrapperTextureData : public dragonBones::TextureData
{
	BIND_CLASS_TYPE_B(WrapperTextureData);

public:
	std::unique_ptr<sf::Sprite> Sprite;

public:
	WrapperTextureData()
	{
		TextureData::_onClear();
	}

	virtual ~WrapperTextureData() override
	{
		TextureData::_onClear();
	}
};

