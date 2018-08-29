#pragma once

/** @file SF3DArmatureProxy.hpp
** @author Piotr Krupa (piotrkrupa06@gmail.com)
**/

#include <SFML/Graphics/Rect.hpp>

#include <dragonBones/DragonBonesHeaders.h>

#include "Szczur/Utility/SFML3D/Drawable.hpp"

DRAGONBONES_NAMESPACE_BEGIN

class SF3DArmatureProxy;
class SF3DEventDispatcher;

class SF3DArmatureDisplay : public sf3d::Drawable
{
private:
	SF3DArmatureProxy* _proxy;

public:
	SF3DArmatureDisplay(const std::string& armatureName, const std::string& dragonBonesName = "", const std::string& skinName = "", const std::string& textureAtlasName = "");
	~SF3DArmatureDisplay();

	void update(float deltaTime);
	void draw(sf3d::RenderTarget& target, sf3d::RenderStates states = sf3d::RenderStates::Default) const override;

	Armature* getArmature() const;
	Animation* getAnimation() const;

	SF3DEventDispatcher* getEvenDispatcher();

	SF3DArmatureProxy* getArmatureProxy() const;

	sf::FloatRect getBoundingBox();
};

DRAGONBONES_NAMESPACE_END
