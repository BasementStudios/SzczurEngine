#pragma once

#include <memory>

#include <dragonBones\DragonBonesHeaders.h>

#include <SFML\Graphics\Sprite.hpp>

DRAGONBONES_NAMESPACE_BEGIN

class SFMLTextureData : public TextureData
{
	BIND_CLASS_TYPE_B(SFMLTextureData);

public:
	sf::Texture*			Texture;
	sf::IntRect				Rect;

public:
	SFMLTextureData()
	{
		_onClear();
	}

	virtual ~SFMLTextureData()
	{
		_onClear();
	}

	void _onClear() override
	{
		Texture = nullptr;

		TextureData::_onClear();
	}
};

DRAGONBONES_NAMESPACE_END
