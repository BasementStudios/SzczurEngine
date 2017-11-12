#include "WrapperArmatureDisplay.h"

#include <SFML\Graphics.hpp>

#include "WrapperDisplay.h"

WrapperArmatureDisplay::WrapperArmatureDisplay()
{
}

WrapperArmatureDisplay::~WrapperArmatureDisplay()
{
}

bool WrapperArmatureDisplay::hasEvent(const std::string& type) const
{
	return false;
}

void WrapperArmatureDisplay::addEvent(const std::string& type, const std::function<void(dragonBones::EventObject*)>& listener)
{
}

void WrapperArmatureDisplay::removeEvent(const std::string& type, const std::function<void(dragonBones::EventObject*)>& listener)
{
}

void WrapperArmatureDisplay::dbInit(dragonBones::Armature* armature)
{
	_armature = armature;
}

void WrapperArmatureDisplay::dbClear()
{
	_armature = nullptr;
}

void WrapperArmatureDisplay::dbUpdate()
{
}

void WrapperArmatureDisplay::dispose(bool disposeProxy)
{
	if (_armature)
	{
		delete _armature;
	}
}

void WrapperArmatureDisplay::_dispatchEvent(const std::string &type, dragonBones::EventObject *value)
{
}

void WrapperArmatureDisplay::render(sf::RenderWindow &window)
{
	auto arr = _armature->getSlots();

	for (auto item : arr)
	//auto item = arr[6];
	{
		auto display = item->getRawDisplay();

		if (!display)
			continue;

		WrapperDisplay* wrapperDisplay = static_cast<WrapperDisplay*>(display);

		wrapperDisplay->render(window, sf::RenderStates::Default);
	}
}
