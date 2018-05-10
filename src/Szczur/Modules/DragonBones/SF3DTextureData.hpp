/** @file SF3DTextureAtlasData.hpp
** @author Piotr Krupa (piotrkrupa06@gmail.com)
** @author Patryk (PsychoX) Ludwikowski <psychoxivi@gmail.com>
**/

#pragma once

#include <memory>

#include <dragonBones/DragonBonesHeaders.h>

#include "Szczur/Utility/SFML3D/Texture.hpp"

DRAGONBONES_NAMESPACE_BEGIN

class SF3DTextureData : public TextureData
{
	BIND_CLASS_TYPE_B(SF3DTextureData);

public:
	sf3d::Texture*			texture;

public:
	SF3DTextureData()
	{
		_onClear();
	}

	virtual ~SF3DTextureData()
	{
		_onClear();
	}

	void _onClear() override
	{
		texture = nullptr;

		TextureData::_onClear();
	}

	void setTexture(sf3d::Texture *texture, dragonBones::Rectangle& region)
	{
		this->texture = texture;
		this->region = region;
	}
};

DRAGONBONES_NAMESPACE_END
