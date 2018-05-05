#pragma once

#include <memory>
#include <vector>

#include <json.hpp>
using Json = nlohmann::json;

namespace rat
{

template <typename T>
size_t typeID()
{
	return reinterpret_cast<size_t>(&typeID<T>);
}

class Component
{
public:

	///
	template <typename... Ts>
	Component(size_t componentID, const std::string& name, Ts... featureIDs)
		: _componentID { componentID }
		, _name { name }
		, _featureIDs { featureIDs... }
	{

	}

	///
	Component(const Component&) = default;

	///
	Component& operator = (const Component&) = default;

	///
	Component(Component&&) = default;

	///
	Component& operator = (Component&&) = default;

	///
	virtual ~Component() = default;

	///
	virtual std::unique_ptr<Component> copy() const = 0;

	///
	const std::string& getName() const
	{
		return _name;
	}

	///
	size_t getComponentID() const
	{
		return _componentID;
	}

	///
	const std::vector<size_t>& getFeatureIDs() const
	{
		return _featureIDs;
	}

	///
	virtual void* getFeature(size_t)
	{
		return nullptr;
	}

	///
	virtual const void* getFeature(size_t) const
	{
		return nullptr;
	}

	///
	virtual void loadFromConfig(const Json& config) = 0;

	///
	virtual void saveToConfig(Json& config) const = 0;

private:

	size_t _componentID;
	std::string _name;
	std::vector<size_t> _featureIDs;

};

}
