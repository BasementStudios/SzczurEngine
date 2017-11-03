#pragma once

#include <dragonBones\DragonBonesHeaders.h>

#include <SFML\Graphics\Sprite.hpp>

class WrapperTextureData : public dragonBones::TextureData
{
	BIND_CLASS_TYPE_B(WrapperTextureData);

public:
	sf::Sprite* Sprite;

public:
	WrapperTextureData() : 
		Sprite(nullptr)
	{
		clear();
	}

	~WrapperTextureData()
	{
		clear();
	}

	void clear()
	{
		TextureData::_onClear();

		if (Sprite)
		{
			delete Sprite;
			Sprite = nullptr;
		}
	}
};

