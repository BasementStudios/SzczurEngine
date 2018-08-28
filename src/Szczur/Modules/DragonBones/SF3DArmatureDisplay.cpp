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
	dispose(false);
}

void SF3DArmatureDisplay::advanceTime(float deltaTime)
{
	if (!_toDelete.empty())
	{
		for (auto toDelete : _toDelete)
		{
			delete toDelete;
		}

		_toDelete.clear();
	}

	_armature->advanceTime(deltaTime);

	for (auto armatureDisplay : _armatureDisplays)
	{
		armatureDisplay->getArmature()->advanceTime(deltaTime);
	}
}

void SF3DArmatureDisplay::dbInit(Armature* armature)
{
	_armature = armature;
}

void SF3DArmatureDisplay::dbClear()
{
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

void SF3DArmatureDisplay::addArmatureDisplay(SF3DArmatureDisplay* value)
{
	_armatureDisplays.push_back(value);
}

void SF3DArmatureDisplay::removeArmatureDisplay(SF3DArmatureDisplay* value)
{
	auto it = std::find_if(_armatureDisplays.begin(), _armatureDisplays.end(), [=] (SF3DArmatureDisplay* display) { return display == value; });

	if (it != _armatureDisplays.end())
	{
		_armatureDisplays.erase(it);
	}
}

void SF3DArmatureDisplay::addDisplay(SF3DDisplay* value)
{
	_displays.push_back(value);
}

void SF3DArmatureDisplay::removeDisplay(SF3DDisplay* value)
{
	auto it = std::find_if(_displays.begin(), _displays.end(), [=] (SF3DDisplay* display) { return display == value; });

	if (it != _displays.end())
	{
		_displays.erase(it);
	}
}

void SF3DArmatureDisplay::addToDelete(SF3DNode* node)
{
	_toDelete.push_back(node);
}

void SF3DArmatureDisplay::dispose(bool disposeProxy)
{
	if (_armature)
	{
		_armature->dispose();
		_armature = nullptr;
	}
}

void SF3DArmatureDisplay::setVisible(bool visible)
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

void SF3DArmatureDisplay::setColor(const dragonBones::ColorTransform& color)
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

void SF3DArmatureDisplay::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
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
