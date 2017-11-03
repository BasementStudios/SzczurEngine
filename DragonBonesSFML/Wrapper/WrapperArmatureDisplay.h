#pragma once

#include <dragonBones\DragonBonesHeaders.h>

#include <SFML\Graphics\RenderWindow.hpp>

class WrapperArmatureDisplay : public dragonBones::IArmatureProxy
{
protected:
	dragonBones::Armature* _armature;

public:
	WrapperArmatureDisplay();
	~WrapperArmatureDisplay();

	bool hasEvent(const std::string& type) const override;
	void addEvent(const std::string& type, const std::function<void(dragonBones::EventObject*)>& listener) override;
	void removeEvent(const std::string& type, const std::function<void(dragonBones::EventObject*)>& listener) override;
	void dbInit(dragonBones::Armature* armature) override;
	void dbClear() override;
	void dbUpdate() override;
	void dispose(bool disposeProxy) override;
	void _dispatchEvent(const std::string &type, dragonBones::EventObject *value);

	dragonBones::Armature* getArmature() const override { return _armature; }
	dragonBones::Animation* getAnimation() const override { return _armature->getAnimation(); }

	void update();
	void render(sf::RenderWindow &window);
};

