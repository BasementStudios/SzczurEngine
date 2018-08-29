/** @file SF3DArmatureProxy.cpp
** @author Piotr Krupa (piotrkrupa06@gmail.com)
**/

#include "SF3DArmatureProxy.hpp"

#include <SFML/Graphics.hpp>

#include "SF3DSlot.hpp"
#include "SF3DDisplay.hpp"

DRAGONBONES_NAMESPACE_BEGIN

void SF3DArmatureProxy::advanceTime(float deltaTime)
{
	_armature->advanceTime(deltaTime);

	for (auto armatureDisplay : _armatureDisplays)
	{
		armatureDisplay->getArmature()->advanceTime(deltaTime);
	}
}

void SF3DArmatureProxy::dbInit(Armature* armature)
{
	_armature = armature;
}

void SF3DArmatureProxy::dbClear()
{
	delete this;
}

void SF3DArmatureProxy::dbUpdate()
{
}

void SF3DArmatureProxy::addDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener)
{
	_dispatcher.addDBEventListener(type, listener);
}

void SF3DArmatureProxy::removeDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener)
{
	_dispatcher.removeDBEventListener(type, listener);
}

void SF3DArmatureProxy::dispatchDBEvent(const std::string& type, EventObject* value)
{
	_dispatcher.dispatchDBEvent(type, value);
}

void SF3DArmatureProxy::addArmatureDisplay(SF3DArmatureProxy* value)
{
	_armatureDisplays.push_back(value);
}

void SF3DArmatureProxy::removeArmatureDisplay(SF3DArmatureProxy* value)
{
	auto it = std::find_if(_armatureDisplays.begin(), _armatureDisplays.end(), [=] (SF3DArmatureProxy* display) { return display == value; });

	if (it != _armatureDisplays.end())
	{
		_armatureDisplays.erase(it);
	}
}

void SF3DArmatureProxy::addDisplay(SF3DDisplay* value)
{
	_displays.push_back(value);
}

void SF3DArmatureProxy::removeDisplay(SF3DDisplay* value)
{
	auto it = std::find_if(_displays.begin(), _displays.end(), [=] (SF3DDisplay* display) { return display == value; });

	if (it != _displays.end())
	{
		_displays.erase(it);
	}
}

void SF3DArmatureProxy::dispose(bool disposeProxy)
{
	if (_armature)
	{
		_armature->dispose();
		_armature = nullptr;
	}
}

void SF3DArmatureProxy::setVisible(bool visible)
{
	for (auto display : _displays)
	{
		display->setVisible(visible);
	}

	for (auto armatureDisplay : _armatureDisplays)
	{
		armatureDisplay->setVisible(visible);
	}
}

void SF3DArmatureProxy::setColor(const dragonBones::ColorTransform& color)
{
	for (auto display : _displays)
	{
		display->setColor(color);
	}

	for (auto armatureDisplay : _armatureDisplays)
	{
		armatureDisplay->setColor(color);
	}
}

void SF3DArmatureProxy::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
{
	states.transform *= this->_transform;

	for (auto display : _displays)
	{
		if (!display)
			continue;

		display->draw(target, states);
	}

	for (auto armatureDisplay : _armatureDisplays)
	{
		if (!armatureDisplay)
			continue;

		armatureDisplay->draw(target, states);
	}
}

sf::FloatRect SF3DArmatureProxy::getBoundingBox()
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
