#pragma once

#include <unordered_map>
#include <functional>
#include <vector>

#include <dragonBones\DragonBonesHeaders.h>

DRAGONBONES_NAMESPACE_BEGIN

class SFMLEventDispatcher
{
	// using container_t = boost::container::flat_map <Hash32_t, std::unique_ptr<tentyp_t>>;

	using Func_t = std::function<void(EventObject*)>;

private:
	std::unordered_map<std::string, std::vector<Func_t>>		_listeners;

public:
	SFMLEventDispatcher() = default;
	~SFMLEventDispatcher() = default;

	void addEvent(const std::string& type, const Func_t& callback)
	{
		_listeners[type].push_back(callback);
	}


	void dispatchEvent(const std::string& type, EventObject* value)
	{
		for (auto& callback : _listeners[type])
		{
			callback(value);
		}
	}
};

DRAGONBONES_NAMESPACE_END
