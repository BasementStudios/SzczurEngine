/** @file SF3DTextureAtlasData.cpp
** @author Piotr Krupa (piotrkrupa06@gmail.com)
**/

#include "SF3DTextureAtlasData.hpp"

#include <memory>

#include "SF3DTextureData.hpp"

DRAGONBONES_NAMESPACE_BEGIN

SF3DTextureAtlasData::SF3DTextureAtlasData()
{
	_onClear();
}

SF3DTextureAtlasData::~SF3DTextureAtlasData()
{
	_onClear();
}

void SF3DTextureAtlasData::setRenderTexture(sf3d::Texture* value)
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
				dragonBones::Rectangle region;
				region.x = textureData->region.x;
				region.y = textureData->region.y;
				region.width = textureData->rotated ? textureData->region.height : textureData->region.width;
				region.height = textureData->rotated ? textureData->region.width : textureData->region.height;

				textureData->setTexture(_renderTexture, region);
			}
		}
	}
}

TextureData* SF3DTextureAtlasData::createTexture() const
{
	return BaseObject::borrowObject<SF3DTextureData>();
}

DRAGONBONES_NAMESPACE_END
