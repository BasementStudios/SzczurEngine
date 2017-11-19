#include "SFMLArmatureDisplay.h"

#include <SFML\Graphics.hpp>

#include "SFMLDisplay.h"

DRAGONBONES_NAMESPACE_BEGIN

SFMLArmatureDisplay::SFMLArmatureDisplay()
{
	_armature = nullptr;
}

SFMLArmatureDisplay::~SFMLArmatureDisplay()
{
	_armature = nullptr;
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

void SFMLArmatureDisplay::addEvent(const std::string& type, const std::function<void(EventObject*)>& listener)
{
	_dispatcher.addEvent(type, listener);
}

void SFMLArmatureDisplay::removeEvent(const std::string& type, const std::function<void(EventObject*)>& listener)
{
}

void SFMLArmatureDisplay::_dispatchEvent(const std::string& type, EventObject* value)
{
	_dispatcher.dispatchEvent(type, value);
}

void SFMLArmatureDisplay::dispose(bool disposeProxy)
{
	if (_armature)
	{
		delete _armature;
		_armature = nullptr;
	}
}

void SFMLArmatureDisplay::render(sf::RenderWindow& window)
{
	auto arr = _armature->getSlots();

	for (auto item : arr)
	{
		if (!item)
			continue;

		auto display = item->getRawDisplay();

		if (!display)
			continue;

		SFMLDisplay* wrapperDisplay = static_cast<SFMLDisplay*>(display);

		wrapperDisplay->render(window, sf::RenderStates::Default);
	}
}

DRAGONBONES_NAMESPACE_END
