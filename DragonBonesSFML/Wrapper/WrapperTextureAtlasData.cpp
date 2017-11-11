#include "WrapperTextureAtlasData.h"

#include <memory>

#include <SFML\Graphics\Rect.hpp>
#include <SFML\System\Vector2.hpp>

#include "WrapperTextureData.h"

WrapperTextureAtlasData::WrapperTextureAtlasData()
{
}

WrapperTextureAtlasData::~WrapperTextureAtlasData()
{
	printf("WrapperTextureAtlasData\n");

	TextureAtlasData::_onClear();
}

void WrapperTextureAtlasData::setRenderTexture(sf::Texture* value)
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
			const auto textureData = static_cast<WrapperTextureData*>(pair.second);

			if (textureData->Sprite == nullptr)
			{
				sf::FloatRect rect(
					textureData->region.x, textureData->region.y,
					textureData->rotated ? textureData->region.height : textureData->region.width,
					textureData->rotated ? textureData->region.width : textureData->region.height
				);

				//printf("[%s] %f %f %f %f\n", textureData->name.c_str(), rect.left, rect.top, rect.width, rect.height);

				sf::Vector2f originSize(rect.width, rect.height);

				auto sprite = std::make_unique<sf::Sprite>(*_renderTexture, sf::IntRect(rect));
				sprite->rotate(textureData->rotated);

				textureData->Sprite = std::move(sprite);
			}
		}
	}
	else
	{
		for (const auto& pair : textures)
		{
			const auto textureData = static_cast<WrapperTextureData*>(pair.second);

			textureData->Sprite.reset();
		}
	}
}

dragonBones::TextureData* WrapperTextureAtlasData::createTexture() const
{
	return BaseObject::borrowObject<WrapperTextureData>();
}
