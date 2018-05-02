/*
*********************************************************************
* File          : SFMLTextureAtlasData.cpp
* Project		: DragonBonesSFML
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
* License   	: MIT License
*********************************************************************
*/

#include "SF3DTextureAtlasData.h"

#include <memory>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "SF3DTextureData.h"

DRAGONBONES_NAMESPACE_BEGIN

SFMLTextureAtlasData::SFMLTextureAtlasData()
{
	_onClear();
}

SFMLTextureAtlasData::~SFMLTextureAtlasData()
{
	_onClear();
}

void SFMLTextureAtlasData::setRenderTexture(sf3d::Texture* value)
{
	if (_renderTexture == value)
	{
		return;
	}

	_renderTexture = value;

	if (_renderTexture != nullptr)
	{
		for (const auto& pair : textures)
		{
			const auto textureData = static_cast<SF3DTextureData*>(pair.second);

			if (textureData->texture == nullptr)
			{
				sf::FloatRect rect(
					textureData->region.x, textureData->region.y,
					textureData->rotated ? textureData->region.height : textureData->region.width,
					textureData->rotated ? textureData->region.width : textureData->region.height
				);


				textureData->setTexture(_renderTexture, rect);
			}
		}
	}
	else
	{
		for (const auto& pair : textures)
		{
			const auto textureData = static_cast<SF3DTextureData*>(pair.second);

			// textureData->Sprite.reset();

			// nothing to release
		}
	}
}

TextureData* SFMLTextureAtlasData::createTexture() const
{
	return BaseObject::borrowObject<SF3DTextureData>();
}

DRAGONBONES_NAMESPACE_END
