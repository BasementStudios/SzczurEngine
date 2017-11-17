#pragma once

#include <memory>

#include <dragonBones\DragonBonesHeaders.h>

#include <SFML\Graphics\Sprite.hpp>

DRAGONBONES_NAMESPACE_BEGIN

class SFMLTextureData : public TextureData
{
	BIND_CLASS_TYPE_B(SFMLTextureData);

public:
	std::unique_ptr<sf::Sprite> Sprite;

public:
	SFMLTextureData()
	{
		TextureData::_onClear();
	}

	virtual ~SFMLTextureData()
	{
		TextureData::_onClear();
	}

	void _onClear() override
	{
		Sprite.reset();
		TextureData::_onClear();
	}
};

DRAGONBONES_NAMESPACE_END
