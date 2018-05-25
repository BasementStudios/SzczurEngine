#include <algorithm>

#include "Scene.hpp"

namespace rat
{

Entity::Entity(Scene* parent, const std::string& group)
	: _id { getUniqueID<Entity>() }
	, _group { group }
	, _name { "unnamed_" + std::to_string(_id) }
	, _parent { parent }
{

}

Entity::Entity(const Entity& rhs)
	: sf3d::Transformable { static_cast<const sf3d::Transformable&>(rhs) }
	, _id { getUniqueID<Entity>() }
	, _group { rhs.getGroup() }
	, _name { rhs.getName() + "_copy_" + std::to_string(_id) }
	, _parent { rhs._parent }
{
	for (const auto& ptr : rhs._holder)
	{
		_holder.emplace_back(ptr->copy(this));
	}
}

Entity& Entity::operator = (const Entity& rhs)
{
	if (this != &rhs)
	{
		static_cast<sf3d::Transformable&>(*this) = static_cast<const sf3d::Transformable&>(rhs);
		_id = getUniqueID<Entity>();
		_group = rhs.getGroup();
		_name = rhs.getName() + "_copy_" + std::to_string(_id);
		_parent = rhs._parent;

		removeAllComponents();

		for (const auto& ptr : rhs._holder)
		{
			_holder.emplace_back(ptr->copy(this));
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

void Entity::setGroup(const std::string& group)
{
	_group = group;
}

const std::string& Entity::getGroup() const
{
	return _group;
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

Component* Entity::addComponent(std::unique_ptr<Component> component)
{
	if (auto ptr = getComponent(component->getComponentID()); ptr != nullptr)
	{
		LOG_WARNING("Entity ( ", getID(), " ) ", std::quoted(getName()), " already has ", ComponentTraits::getNameFromIdentifier(ptr->getComponentID()), " component, existing one returned");

		return ptr;
	}

	return _holder.emplace_back(std::move(component)).get();
}

Component* Entity::addComponent(Hash64_t componentID)
{
	return addComponent(ComponentTraits::createFromComponentID(this, componentID));
}

bool Entity::removeComponent(Hash64_t componentID)
{
	if (auto it = _findByComponentID(componentID); it != _holder.end())
	{
		_holder.erase(it);

		return true;
	}

	return false;
}

void Entity::removeAllComponents()
{
	_holder.clear();
}

Component* Entity::getComponent(Hash64_t componentID) const
{
	if (auto it = _findByComponentID(componentID); it != _holder.end())
	{
		return it->get();
	}

	return nullptr;
}

bool Entity::hasComponent(Hash64_t componentID) const
{
	return _findByComponentID(componentID) != _holder.end();
}

Entity::ComponentsHolder_t& Entity::getComponents()
{
	return _holder;
}

const Entity::ComponentsHolder_t& Entity::getComponents() const
{
	return _holder;
}

void Entity::loadFromConfig(const Json& config, bool withNewID)
{
	_id = withNewID ? getUniqueID<Entity>() : config["id"].get<size_t>();
	_name = config["name"].get<std::string>();

	const Json& components = config["components"];

	for (const Json& component : components)
	{
		addComponent(static_cast<Hash64_t>(component["id"]))->loadFromConfig(component);
	}

	trySettingInitialUniqueID<Entity>(_id);
}

void Entity::saveToConfig(Json& config) const
{
	config["id"] = getID();
	config["name"] = getName();
	config["components"] = Json::array();

	for (const auto& component : _holder)
	{
		config["components"].push_back(Json::object());
		Json& comp = config["components"].back();

		component->saveToConfig(comp);
	}
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
