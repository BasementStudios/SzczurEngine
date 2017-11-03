#include "WrapperTextureAtlasData.h"

#include <SFML\Graphics\Rect.hpp>
#include <SFML\System\Vector2.hpp>

#include "WrapperTextureData.h"

WrapperTextureAtlasData::WrapperTextureAtlasData()
{
}

WrapperTextureAtlasData::~WrapperTextureAtlasData()
{
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

				//_renderTexture->retain();

				sf::FloatRect rect(
					textureData->region.x, textureData->region.y,
					textureData->rotated ? textureData->region.height : textureData->region.width,
					textureData->rotated ? textureData->region.width : textureData->region.height
				);

				//printf("%s\n", textureData->name.c_str());

				printf("[%s] %f %f %f %f\n", textureData->name.c_str(), rect.left, rect.top, rect.width, rect.height);

				sf::Vector2f originSize(rect.width, rect.height);

				sf::Sprite *sprite = new sf::Sprite(*_renderTexture, sf::IntRect(rect));
				sprite->rotate(textureData->rotated);

				textureData->Sprite = sprite;

				//textureData->Sprite = cocos2d::SpriteFrame::createWithTexture(_renderTexture, rect, textureData->rotated, offset, originSize);
				//textureData->Sprite->retain();
			}
		}
	}
	else
	{
		for (const auto& pair : textures)
		{
			const auto textureData = static_cast<WrapperTextureData*>(pair.second);

			if (textureData->Sprite)
			{
				delete textureData->Sprite;
				textureData->Sprite = nullptr;
			}
		}
	}
}

void WrapperTextureAtlasData::clear()
{
	TextureAtlasData::_onClear();

	if (_renderTexture)
	{
		delete _renderTexture;
		_renderTexture = nullptr;
	}
}

dragonBones::TextureData* WrapperTextureAtlasData::createTexture() const
{
	return BaseObject::borrowObject<WrapperTextureData>();
}
