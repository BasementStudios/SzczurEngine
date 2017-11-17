#include "SFMLArmatureDisplay.h"

#include <SFML\Graphics.hpp>

#include "SFMLDisplay.h"

DRAGONBONES_NAMESPACE_BEGIN

SFMLArmatureDisplay::SFMLArmatureDisplay()
{
}

SFMLArmatureDisplay::~SFMLArmatureDisplay()
{
}

bool SFMLArmatureDisplay::hasEvent(const std::string& type) const
{
	return true;
}

void SFMLArmatureDisplay::addEvent(const std::string& type, const std::function<void(EventObject*)>& listener)
{
}

void SFMLArmatureDisplay::removeEvent(const std::string& type, const std::function<void(EventObject*)>& listener)
{
}

void SFMLArmatureDisplay::dbInit(Armature* armature)
{
	_armature = armature;
}

void SFMLArmatureDisplay::dbClear()
{
	_armature = nullptr;
}

void SFMLArmatureDisplay::dbUpdate()
{
}

void SFMLArmatureDisplay::dispose(bool disposeProxy)
{
	if (_armature)
	{
		delete _armature;
	}
}

void SFMLArmatureDisplay::_dispatchEvent(const std::string &type, EventObject *value)
{
}

void SFMLArmatureDisplay::render(sf::RenderWindow &window)
{
	auto arr = _armature->getSlots();

	for (auto item : arr)
	{
		auto display = item->getRawDisplay();

		if (!display)
			continue;

		SFMLDisplay* wrapperDisplay = static_cast<SFMLDisplay*>(display);

		wrapperDisplay->render(window, sf::RenderStates::Default);
	}
}

DRAGONBONES_NAMESPACE_END
