
/** @file SF3DTextureAtlasData.cpp
 ** @author Piotr Krupa (piotrkrupa06@gmail.com)
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi@gmail.com>
 **/

#include "SF3DTextureAtlasData.hpp"

#include "Szczur/Utility/SFML3D/Texture.hpp"

#include "SF3DTextureData.hpp"

DRAGONBONES_NAMESPACE_BEGIN

/* Properties */
sf3d::Texture* SF3DTextureAtlasData::getRenderTexture() const 
{
	return texture;
}
void SF3DTextureAtlasData::setRenderTexture(sf3d::Texture* value)
{
	if (texture == value) {
		return;
	}
	texture = value;

	if (texture != nullptr) {
		// Set texture to related texture data objects
		for (const auto& pair : textures) {
			const auto textureData = static_cast<SF3DTextureData*>(pair.second);

			if (textureData->texture == nullptr) {
				textureData->texture = texture;
			}
		}
	}
}



/* Operators */
SF3DTextureAtlasData::SF3DTextureAtlasData()
{
	_onClear();
}

SF3DTextureAtlasData::~SF3DTextureAtlasData()
{
	_onClear();
}



/* Methods */
TextureData* SF3DTextureAtlasData::createTexture() const
{
	return BaseObject::borrowObject<SF3DTextureData>();
}

DRAGONBONES_NAMESPACE_END
