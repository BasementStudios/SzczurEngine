#include <algorithm>

#include "Scene.hpp"

namespace rat
{

Entity::Entity(Scene* parent)
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

void Entity::update(float /*deltaTime*/)
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

Scene* Entity::getScene()
{
	return _parent;
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
	_id = config["id"];
	_name = config["name"].get<std::string>();
	setPosition({
		config["position"]["x"].get<float>(),
		config["position"]["y"].get<float>(),
		config["position"]["z"].get<float>()
	});
	setRotation({
		config["rotation"]["x"].get<float>(),
		config["rotation"]["y"].get<float>(),
		config["rotation"]["z"].get<float>()
	});
	setOrigin({
		config["origin"]["x"].get<float>(),
		-config["origin"]["y"].get<float>(),
		config["origin"]["z"].get<float>()
	});
	setScale({
		config["scale"]["x"].get<float>(),
		config["scale"]["y"].get<float>(),
		config["scale"]["z"].get<float>()
	});

	const Json& components = config["components"];

	for (const Json& component : components)
	{
		addComponent(static_cast<Hash64_t>(component["id"]))->loadFromConfig(component);
	}
}

void Entity::saveToConfig(Json& config) const
{
	config["id"] = getID();
	config["name"] = getName();
	config["components"] = Json::array();


	config["position"]["x"] = getPosition().x;
	config["position"]["y"] = getPosition().y;
	config["position"]["z"] = getPosition().z;

	config["rotation"]["x"] = getRotation().x;
	config["rotation"]["y"] = getRotation().y;
	config["rotation"]["z"] = getRotation().z;

	config["origin"]["x"] = getOrigin().x;
	config["origin"]["y"] = getOrigin().y;
	config["origin"]["z"] = getOrigin().z;

	config["scale"]["x"] = getScale().x;
	config["scale"]["y"] = getScale().y;
	config["scale"]["z"] = getScale().z;


	

	for (const auto& component : _holder)
	{
		config["components"].push_back(Json::object());
		Json& comp = config["components"].back();

		component->saveToConfig(comp);
	}
}

size_t Entity::_getUniqueID()
{
	static size_t id = 0;

	return ++id;
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

typename Entity::ComponentsHolder_t::iterator Entity::_findByFeature(Component::Feature_e feature)
{
	return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
		return arg->getFeatures() & feature;
	});
}

typename Entity::ComponentsHolder_t::const_iterator Entity::_findByFeature(Component::Feature_e feature) const
{
	return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
		return arg->getFeatures() & feature;
	});
}

}
