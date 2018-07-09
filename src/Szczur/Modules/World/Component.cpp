#include "Component.hpp"

#include <memory>
#include <vector>

#include <nlohmann/json.hpp>

namespace rat
{

Component::Component(Entity* parent, Hash64_t id, const std::string& name, size_t features)
	: _id { id }
	, _features { features }
	, _name { name }
	, _parent { parent }
{

}

Hash64_t Component::getComponentID() const
{
	return _id;
}

void* Component::getFeature(Component::Feature_e)
{
	return nullptr;
}

const void* Component::getFeature(Component::Feature_e) const
{
	return nullptr;
}

size_t Component::getFeatures() const
{
	return _features;
}

const std::string& Component::getName() const
{
	return _name;
}

void Component::setEntity(Entity* entity)
{
	_parent = entity;
}

Entity* Component::getEntity()
{
	return _parent;
}

const Entity* Component::getEntity() const
{
	return _parent;
}

void Component::loadFromConfig(nlohmann::json& config)
{
	_id = config["id"];
	_features = config["features"];
	_name = config["name"].get<std::string>();

	trySettingInitialUniqueID<Component>(_id);
}

void Component::saveToConfig(nlohmann::json& config) const
{
	config["id"] = getComponentID();
	config["features"] = getFeatures();
	config["name"] = getName();
}

}
