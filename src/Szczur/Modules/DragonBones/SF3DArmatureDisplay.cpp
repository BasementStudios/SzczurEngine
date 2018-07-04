/** @file SF3DArmatureDisplay.cpp
** @author Piotr Krupa (piotrkrupa06@gmail.com)
**/

#include "SF3DArmatureDisplay.hpp"

#include <SFML/Graphics.hpp>

#include "SF3DSlot.hpp"
#include "SF3DDisplay.hpp"

DRAGONBONES_NAMESPACE_BEGIN

SF3DArmatureDisplay::SF3DArmatureDisplay()
{
	_armature = nullptr;
}

SF3DArmatureDisplay::~SF3DArmatureDisplay()
{
	if (_armature)
	{
		delete _armature;
		_armature = nullptr;
	}
}

void SF3DArmatureDisplay::dbInit(Armature* armature)
{
	_armature = armature;
}

void SF3DArmatureDisplay::dbClear()
{
	_armature = nullptr;
}

void SF3DArmatureDisplay::dbUpdate()
{
}

void SF3DArmatureDisplay::addDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener)
{
	_dispatcher.addDBEventListener(type, listener);
}

void SF3DArmatureDisplay::removeDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener)
{
}

void SF3DArmatureDisplay::dispatchDBEvent(const std::string& type, EventObject* value)
{
	_dispatcher.dispatchDBEvent(type, value);
}

void SF3DArmatureDisplay::dispose(bool disposeProxy)
{
	if (_armature)
	{
		delete _armature;
		_armature = nullptr;
	}
}

void SF3DArmatureDisplay::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
{
	for (auto slot : _armature->getSlots())
	{
		if (!slot)
			continue;

		auto display = static_cast<SF3DDisplay*>(slot->getRawDisplay());
		
		if (!display)
			continue;

		display->draw(target, states);
	}
}

sf::FloatRect SF3DArmatureDisplay::getBoundingBox()
{
	auto slots = _armature->getSlots();
	bool isFirst = true;

	sf::Vector2f min;
	sf::Vector2f max;

	for (const auto slot : _armature->getSlots())
	{
		if (!slot->getVisible() || !slot->getDisplay())
		{
			continue;
		}

		auto display = static_cast<SF3DDisplay*>(slot->getRawDisplay());
		const auto bounds = display->getBoundingBox();
		if (isFirst)
		{
			isFirst = false;
			min = { bounds.left, bounds.top };
			max = { bounds.left + bounds.width, bounds.top + bounds.height };
		}
		else
		{
			min.x = std::min(min.x, bounds.left);
			min.y = std::min(min.y, bounds.top);
			max.x = std::max(max.x, bounds.left + bounds.width);
			max.y = std::max(max.y, bounds.top + bounds.height);
		}
	}

	return sf::FloatRect(min, max - min);
}

DRAGONBONES_NAMESPACE_END
