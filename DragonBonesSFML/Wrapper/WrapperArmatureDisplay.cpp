#include "WrapperArmatureDisplay.h"

#include <SFML\Graphics.hpp>

#include "WrapperSprite.h"

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
	/*auto lambda = [callback] (cocos2d::EventCustom* event) -> void
	{
		callback(static_cast<EventObject*>(event->getUserData()));
	};
	_dispatcher->addCustomEventListener(type, lambda);*/
}

void WrapperArmatureDisplay::removeEvent(const std::string& type, const std::function<void(dragonBones::EventObject*)>& listener)
{
	// TODO
	// _dispatcher->removeCustomEventListeners(type);
}

void WrapperArmatureDisplay::dbInit(dragonBones::Armature* armature)
{
	_armature = armature;
}

void WrapperArmatureDisplay::dbClear()
{
	// setEventDispatcher(cocos2d::Director::getInstance()->getEventDispatcher());

	_armature = nullptr;
}

void WrapperArmatureDisplay::dbUpdate()
{
	const auto drawed = dragonBones::DragonBones::debugDraw;
	if (drawed)
	{
		if (drawed)
		{

		}
		else
		{
			// TODO
		}
	}
}

void WrapperArmatureDisplay::dispose(bool disposeProxy)
{
	if (_armature)
	{
		delete _armature;
		_armature = nullptr;
	}
}

void WrapperArmatureDisplay::_dispatchEvent(const std::string &type, dragonBones::EventObject *value)
{
	//_dispatcher->dispatchCustomEvent(type, value);
}

void WrapperArmatureDisplay::update()
{
}

void WrapperArmatureDisplay::render(sf::RenderWindow &window)
{
	auto arr = _armature->getSlots();

	for (auto item : arr)
	//auto item = arr[12];
	{
		auto display = item->getRawDisplay();

		if (!display)
		{
			//assert(false);
			continue;
		}

		WrapperSprite* sprite = static_cast<WrapperSprite*>(display);

		sprite->render(window);
	}
}
