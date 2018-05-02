
/** @file SF3DTextureAtlasData.hpp
 ** @author Piotr Krupa (piotrkrupa06@gmail.com)
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi@gmail.com>
 **/

#pragma once

#include <dragonBones/DragonBonesHeaders.h>

#include "Szczur/Utility/SFML3D/Texture.hpp"

DRAGONBONES_NAMESPACE_BEGIN

/** class SF3DTextureAtlasData
 **/
class SF3DTextureAtlasData : public TextureAtlasData
{
	BIND_CLASS_TYPE_B(SF3DTextureAtlasData);



	/* Fields */
private:
	sf3d::Texture* texture;



	/* Properties */
public:
	/// RenderTexture
	sf3d::Texture* getRenderTexture() const;
	void setRenderTexture(sf3d::Texture* value);



	/* Operators */
public:
	SF3DTextureAtlasData();

	~SF3DTextureAtlasData();



	/* Methods */
public:
	virtual TextureData* createTexture() const override;
};

DRAGONBONES_NAMESPACE_END
