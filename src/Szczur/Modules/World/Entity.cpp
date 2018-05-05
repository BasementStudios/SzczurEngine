#include "Entity.hpp"

#include <algorithm>

#include "Scene.hpp"

namespace rat
{

Entity::Entity(const Scene* parent)
	: _id { _getUniqueID() }
	, _name { "unnamed_" + std::to_string(_id) }
	, _parent { parent }
{

}

Entity::Entity(const Entity& rhs)
{
	for (const auto& ptr : rhs._holder)
	{
		_holder.emplace_back(ptr->copy());
	}
}

Entity& Entity::operator = (const Entity& rhs)
{
	if (this != &rhs)
	{
		removeAllComponents();

		for (const auto& ptr : rhs._holder)
		{
			_holder.emplace_back(ptr->copy());
		}
	}

	return *this;
}

void Entity::update()
{

}

void Entity::render()
{

}

size_t Entity::getID() const
{
	return _id;
}

void Entity::setName(const std::string& name)
{
	_name = name;
}

const std::string& Entity::getName() const
{
	return _name;
}

const Scene* Entity::getScene() const
{
	return _parent;
}

void Entity::removeAllComponents()
{
	_holder.clear();
}

const Entity::ComponentsHolder_t& Entity::getComponents() const
{
	return _holder;
}

void Entity::loadFromConfig(const Json& config)
{
	// for (const Json& component : config["components"])
	// {
	// 	addComponent(component["name"].get<std::string>());
	//
	// 	getComponent(component["name"].get<std::string>())->loadFromConfig(component);
	// }
}

void Entity::saveToConfig(Json& config) const
{
	// config["name"] = getName();
	// config["components"] = Json::array();
	//
	// for (const auto& component : getComponents())
	// {
	// 	config["components"].push_back(Json::object());
	// 	Json& currComponent = config["components"].back();
	//
	// 	component->saveToConfig(currComponent);
	// }
}

size_t Entity::_getUniqueID()
{
	static size_t id = 0;

	return id++;
}

typename Entity::ComponentsHolder_t::iterator Entity::_findByComponentID(size_t id)
{
	return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
		return arg->getComponentID() == id;
	});
}

typename Entity::ComponentsHolder_t::const_iterator Entity::_findByComponentID(size_t id) const
{
	return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
		return arg->getComponentID() == id;
	});
}

typename Entity::ComponentsHolder_t::iterator Entity::_findByFeatureID(size_t featureID)
{
	return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
		const auto& featureIDs = arg->getFeatureIDs();
		return std::find(featureIDs.begin(), featureIDs.end(), featureID) != featureIDs.end();
	});
}

typename Entity::ComponentsHolder_t::const_iterator Entity::_findByFeatureID(size_t featureID) const
{
	return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
		const auto& featureIDs = arg->getFeatureIDs();
		return std::find(featureIDs.begin(), featureIDs.end(), featureID) != featureIDs.end();
	});
}

}
