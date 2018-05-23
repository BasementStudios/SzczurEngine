#pragma once

#include <memory>
#include <vector>

#include <json.hpp>
using Json = nlohmann::json;

#include "UniqueID.hpp"
#include "Szczur/Utility/Convert/Hash.hpp"

namespace rat
{

// FWD
class Entity;

class Component
{
public:

	enum Feature_e
	{
		Drawable = 1
	};

	///
	Component(Entity* parent, Hash64_t id, const std::string& name, size_t features = 0);

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
	virtual std::unique_ptr<Component> copy(Entity* newParent) const = 0;

	///
	Hash64_t getComponentID() const;

	///
	virtual void* getFeature(Component::Feature_e);

	///
	virtual const void* getFeature(Component::Feature_e) const;

	///
	size_t getFeatures() const;

	///
	const std::string& getName() const;

	///
	void setEntity(Entity* entity);

	///
	Entity* getEntity();

	///
	const Entity* getEntity() const;

	///
	virtual void loadFromConfig(const Json& config);

	///
	virtual void saveToConfig(Json& config) const;

	///
	void updateIDs();

private:

	Hash64_t _id;
	size_t _features;
	std::string _name;
	Entity* _parent;

};

}
