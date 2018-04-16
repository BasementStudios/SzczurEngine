
/** @file SF3DEventDispatcher.hpp
 ** @author Piotr Krupa (piotrkrupa06@gmail.com)
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi@gmail.com>
 **/

#pragma once

#include <unordered_map>
#include <functional>
#include <vector>

#include <dragonBones/DragonBonesHeaders.h>

DRAGONBONES_NAMESPACE_BEGIN

/** @class SF3DEventDispatcher
 **/
class SF3DEventDispatcher : public IEventDispatcher
{
	/* Types */
public:
	using Callback_t = std::function<void(EventObject*)>;



	/* Fields */
protected:
	std::unordered_map<std::string, std::vector<Callback_t>> listeners;



	/* Operators */
public:
	SF3DEventDispatcher() = default;
	~SF3DEventDispatcher() = default;



	/* Methods */ // @todo , move implementation to `cpp` files.
public:
	virtual void addDBEventListener(const std::string& type, const Callback_t& listener) override
	{
		this->listeners[type].push_back(listener);
	}

	virtual void removeDBEventListener(const std::string& type, const Callback_t& listener) override
	{
		// @todo . remove ;F
	}

	virtual void dispatchDBEvent(const std::string& type, EventObject* value) override
	{
		for (auto& listener : this->listeners[type]) {
			listener(value);
		}
	}

	virtual bool hasDBEventListener(const std::string& type) const
	{
		return true;
	};
};

DRAGONBONES_NAMESPACE_END
