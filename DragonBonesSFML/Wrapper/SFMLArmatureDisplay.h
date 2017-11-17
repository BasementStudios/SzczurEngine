#pragma once

#include <dragonBones\DragonBonesHeaders.h>

#include <SFML\Graphics\RenderWindow.hpp>

DRAGONBONES_NAMESPACE_BEGIN

class SFMLArmatureDisplay : public IArmatureProxy
{
protected:
	Armature*		_armature;

public:
	SFMLArmatureDisplay();
	~SFMLArmatureDisplay();

	bool hasEvent(const std::string& type) const override;
	void addEvent(const std::string& type, const std::function<void(EventObject*)>& listener) override;
	void removeEvent(const std::string& type, const std::function<void(EventObject*)>& listener) override;
	void dbInit(Armature* armature) override;
	void dbClear() override;
	void dbUpdate() override;
	void dispose(bool disposeProxy) override;
	void _dispatchEvent(const std::string &type, EventObject *value);

	Armature* getArmature() const override { return _armature; }
	Animation* getAnimation() const override { return _armature->getAnimation(); }

	void render(sf::RenderWindow &window);
};

DRAGONBONES_NAMESPACE_END
