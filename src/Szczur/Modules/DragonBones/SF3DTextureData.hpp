
/** @file TextureData.hpp
 ** @author Piotr Krupa (piotrkrupa06@gmail.com)
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi@gmail.com>
 **/

#pragma once

#include <dragonBones/DragonBonesHeaders.h>

#include <glm/glm.hpp> // vec2

#include "Szczur/Utility/SFML3D/Texture.hpp"

DRAGONBONES_NAMESPACE_BEGIN

/** class SF3DTextureData
 **/
class SF3DTextureData : public TextureData
{
	BIND_CLASS_TYPE_B(SF3DTextureData);

	/* Fields */
public:
	sf3d::Texture* texture;



	/* Operators */
public:
	SF3DTextureData()
	{
		_onClear();
	}

	virtual ~SF3DTextureData()
	{
		_onClear();
	}



	/* Methods */
public:
	void _onClear() override
	{
		texture = nullptr;

		TextureData::_onClear();
	}

	void setTexture(sf3d::Texture* value) // @todo , move impl to .cpp
	{
		// Use whole texture for each element // @todo . poprawić atlas - kilka tekstur, i regiony z nich pobierane dopiero
		region.x = 0.f;
		region.y = 0.f;
		region.width = 1.f;
		region.height = 1.f;

		texture = value;
	}

	const glm::vec2& getSize() const
	{
		return this->texture->getSize();
	}
};

DRAGONBONES_NAMESPACE_END
