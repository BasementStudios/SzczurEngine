#pragma once

/** @file SF3DArmatureProxy.hpp
 ** @author Piotr Krupa (piotrkrupa06@gmail.com)
 **/

#include <dragonBones/DragonBonesHeaders.h>
#include <SFML/Graphics/Rect.hpp>

#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"

#include "SF3DNode.hpp"
#include "SF3DEventDispatcher.hpp"
#include "SF3DFactory.hpp"

DRAGONBONES_NAMESPACE_BEGIN

class SF3DDisplay;

class SF3DArmatureProxy : public SF3DNode, public IArmatureProxy
{
	friend SF3DFactory;

protected:
	Armature*						_armature = nullptr;
	SF3DEventDispatcher				_dispatcher;

	std::vector<SF3DArmatureProxy*>	_armatureDisplays;
	std::vector<SF3DDisplay*>		_displays;

protected:
	SF3DArmatureProxy() = default;
	~SF3DArmatureProxy() = default;

public:
	void advanceTime(float deltaTime);

	bool hasDBEventListener(const std::string& type) const override { return true; }
	void addDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener) override;
	void removeDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener) override;
	void dispatchDBEvent(const std::string& type, EventObject* value) override;

	void addArmatureDisplay(SF3DArmatureProxy* value);
	void removeArmatureDisplay(SF3DArmatureProxy* value);

	void addDisplay(SF3DDisplay* value);
	void removeDisplay(SF3DDisplay* value);

	void dbInit(Armature* armature) override;
	void dbClear() override;
	void dbUpdate() override;

	void dispose(bool disposeProxy) override;
	
	Armature* getArmature() const override { return _armature; }
	Animation* getAnimation() const override { return _armature->getAnimation(); }

	void setVisible(bool visible) override;
	void setColor(const dragonBones::ColorTransform& color) override;
	void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const override;

	sf::FloatRect getBoundingBox() override;

};

DRAGONBONES_NAMESPACE_END
