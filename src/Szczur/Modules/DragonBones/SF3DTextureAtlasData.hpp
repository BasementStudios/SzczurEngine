/** @file SF3DTextureAtlasData.hpp
** @author Piotr Krupa (piotrkrupa06@gmail.com)
**/

#pragma once

#include <dragonBones/DragonBonesHeaders.h>

#include "Szczur/Utility/SFML3D/Texture.hpp"

DRAGONBONES_NAMESPACE_BEGIN

class SF3DTextureAtlasData : public TextureAtlasData
{
	BIND_CLASS_TYPE_B(SF3DTextureAtlasData);

private:
	sf3d::Texture* _renderTexture;

public:
	SF3DTextureAtlasData();
	~SF3DTextureAtlasData();

	virtual TextureData* createTexture() const override;

	sf3d::Texture* getRenderTexture() const { return _renderTexture; }

	void setRenderTexture(sf3d::Texture* value);
};

DRAGONBONES_NAMESPACE_END
