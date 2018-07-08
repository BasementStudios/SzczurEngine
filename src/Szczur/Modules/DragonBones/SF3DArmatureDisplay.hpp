/** @file SF3DArmatureDisplay.hpp
** @author Piotr Krupa (piotrkrupa06@gmail.com)
**/

#pragma once

#include <dragonBones/DragonBonesHeaders.h>

#include "Szczur/Utility/SFML3D/Drawable.hpp"

#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/VertexArray.hpp"
#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"

#include "SF3DEventDispatcher.hpp"

#include <glm/glm.hpp>

DRAGONBONES_NAMESPACE_BEGIN

class SF3DArmatureDisplay : public IArmatureProxy, public sf3d::Drawable
{
protected:
	Armature*									_armature = nullptr;
	SFMLEventDispatcher							_dispatcher;

public:
	SF3DArmatureDisplay();
	~SF3DArmatureDisplay();

	bool hasDBEventListener(const std::string& type) const override { return true; }
	void addDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener) override;
	void removeDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener) override;
	void dispatchDBEvent(const std::string& type, EventObject* value) override;

	void dbInit(Armature* armature) override;
	void dbClear() override;
	void dbUpdate() override;

	void dispose(bool disposeProxy) override;
	
	Armature* getArmature() const override { return _armature; }
	Animation* getAnimation() const override { return _armature->getAnimation(); }

	void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const;

	sf::FloatRect getBoundingBox();
};

DRAGONBONES_NAMESPACE_END
