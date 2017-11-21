#include "SFMLTextureAtlasData.h"

#include <memory>

#include <SFML\Graphics\Rect.hpp>
#include <SFML\System\Vector2.hpp>

#include "SFMLTextureData.h"

DRAGONBONES_NAMESPACE_BEGIN

SFMLTextureAtlasData::SFMLTextureAtlasData()
{
	_onClear();
}

SFMLTextureAtlasData::~SFMLTextureAtlasData()
{
	_onClear();
}

void SFMLTextureAtlasData::setRenderTexture(sf::Texture* value)
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
			const auto textureData = static_cast<SFMLTextureData*>(pair.second);

			if (textureData->texture == nullptr)
			{
				sf::IntRect rect(
					textureData->region.x, textureData->region.y,
					textureData->rotated ? textureData->region.height : textureData->region.width,
					textureData->rotated ? textureData->region.width : textureData->region.height
				);

				//printf("[%s] %f %f %f %f\n", textureData->name.c_str(), rect.left, rect.top, rect.width, rect.height);

				textureData->texture = _renderTexture;
				textureData->textureRect = std::move(rect);
			}
		}
	}
	else
	{
		for (const auto& pair : textures)
		{
			const auto textureData = static_cast<SFMLTextureData*>(pair.second);

			// textureData->Sprite.reset();

			// nothing to release
		}
	}
}

TextureData* SFMLTextureAtlasData::createTexture() const
{
	return BaseObject::borrowObject<SFMLTextureData>();
}

DRAGONBONES_NAMESPACE_END
