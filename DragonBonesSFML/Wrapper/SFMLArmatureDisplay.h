#pragma once

#include <dragonBones\DragonBonesHeaders.h>

#include <SFML\Graphics\RenderWindow.hpp>

#include "SFMLEventDispatcher.h"

DRAGONBONES_NAMESPACE_BEGIN

class SFMLArmatureDisplay : public IArmatureProxy
{
protected:
	Armature*									_armature;
	SFMLEventDispatcher							_dispatcher;

	sf::Vector2f								_pos;

public:
	SFMLArmatureDisplay();
	~SFMLArmatureDisplay();

	bool hasEvent(const std::string& type) const override { return true; }
	void addEvent(const std::string& type, const std::function<void(EventObject*)>& listener) override;
	void removeEvent(const std::string& type, const std::function<void(EventObject*)>& listener) override;
	void _dispatchEvent(const std::string& type, EventObject* value);

	void dbInit(Armature* armature) override;
	void dbClear() override;
	void dbUpdate() override;

	void dispose(bool disposeProxy) override;
	
	Armature* getArmature() const override { return _armature; }
	Animation* getAnimation() const override { return _armature->getAnimation(); }

	void setPosition(const sf::Vector2f& pos) { _pos = pos; }
	auto& getPosition() { return _pos; }

	void render(sf::RenderWindow& window);
};

DRAGONBONES_NAMESPACE_END
