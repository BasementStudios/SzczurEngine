#pragma once

#include <algorithm>

#include "Components.hpp"

#include <Szczur/Utility/SFML3D/Transformable.hpp>

namespace rat
{

class Entity : public sf3d::Transformable
{
public:

	using ComponentsHolder_t = std::vector<std::unique_ptr<Component>>;

	///
	Entity(const std::string& name)
		: _name{ name }
	{

	}

	///
	Entity(const Entity& rhs)
	{
		for (const auto& ptr : rhs._holder)
		{
			_holder.emplace_back(ptr->copy());
		}
	}

	///
	Entity& operator = (const Entity& rhs)
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

	///
	Entity(Entity&&) = default;

	///
	Entity& operator = (Entity&&) = default;

	///
	void setName(const std::string& name)
	{
		_name = name;
	}

	///
	const std::string& getName() const
	{
		return _name;
	}

	///
	bool addComponent(size_t componentID)
	{
		if (hasComponent(componentID)) return false;
		_holder.emplace_back(ComponentTraits::createFromComponentID(componentID));

		return true;
	}

	///
	bool addComponent(const std::string& name)
	{
		return addComponent(ComponentTraits::getIdentifierFromName(name));
	}

	///
	template <typename T>
	bool addComponent()
	{
		return addComponent(typeID<T>());
	}

	///
	bool addComponent(std::unique_ptr<Component> ptr)
	{
		if (hasComponent(ptr->getComponentID())) return false;

		_holder.emplace_back(std::move(ptr));

		return true;
	}

	///
	void removeAllComponents()
	{
		_holder.clear();
	}

	///
	bool removeComponent(size_t componentID)
	{
		if (auto it = _findByComponentID(componentID); it != _holder.end())
		{
			_holder.erase(it);

			return true;
		}

		return false;
	}

	///
	bool removeComponent(const std::string& name)
	{
		return removeComponent(ComponentTraits::getIdentifierFromName(name));
	}

	///
	template <typename T>
	bool removeComponent()
	{
		return removeComponent(typeID<T>());
	}

	///
	Component* getComponent(size_t componentID) const
	{
		if (auto it = _findByComponentID(componentID); it != _holder.end())
		{
			return it->get();
		}

		return nullptr;
	}

	///
	Component* getComponent(const std::string& name) const
	{
		return getComponent(ComponentTraits::getIdentifierFromName(name));
	}

	///
	template <typename T>
	Component* getComponent() const
	{
		return getComponent(typeID<T>());
	}

	///
	template <typename T>
	T* getComponentAs() const
	{
		return static_cast<T*>(getComponent(typeID<T>()));
	}

	///
	template <typename T>
	T* getFeature()
	{
		if (auto it = _findByFeatureID(typeID<T>()); it != _holder.end())
		{
			return static_cast<T*>(it->get()->getFeature(typeID<T>()));
		}

		return nullptr;
	}

	///
	template <typename T>
	const T* getFeature() const
	{
		if (auto it = _findByFeatureID(typeID<T>()); it != _holder.end())
		{
			return static_cast<T*>(it->get()->getFeature(typeID<T>()));
		}

		return nullptr;
	}

	///
	bool hasComponent(size_t id) const
	{
		return _findByComponentID(id) != _holder.end();
	}

	///
	bool hasComponent(const std::string& name) const
	{
		return hasComponent(ComponentTraits::getIdentifierFromName(name));
	}

	///
	template <typename T>
	bool hasComponent() const
	{
		return hasComponent(typeID<T>());
	}

	///
	const ComponentsHolder_t& getComponents() const
	{
		return _holder;
	}

	///
	virtual void loadFromConfig(const Json& config)
	{
		for (const Json& component : config["components"])
		{
			addComponent(component["name"].get<std::string>());

			getComponent(component["name"].get<std::string>())->loadFromConfig(component);
		}
	}

	///
	void saveToConfig(Json& config) const
	{
		config["name"] = getName();
		config["components"] = Json::array();

		for (const auto& component : getComponents())
		{
			config["components"].push_back(Json::object());
			Json& currComponent = config["components"].back();

			component->saveToConfig(currComponent);
		}
	}

	#ifdef EDITOR
	bool isLocked() const {
		return _locked;
	}
	void setLock(bool lock) {
		_locked = lock;
	}
	#endif

private:

	///
	typename ComponentsHolder_t::iterator _findByComponentID(size_t id)
	{
		return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
			return arg->getComponentID() == id;
		});
	}

	///
	typename ComponentsHolder_t::const_iterator _findByComponentID(size_t id) const
	{
		return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
			return arg->getComponentID() == id;
		});
	}

	///
	typename ComponentsHolder_t::iterator _findByFeatureID(size_t featureID)
	{
		return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
			const auto& featureIDs = arg->getFeatureIDs();
			return std::find(featureIDs.begin(), featureIDs.end(), featureID) != featureIDs.end();
		});
	}

	///
	typename ComponentsHolder_t::const_iterator _findByFeatureID(size_t featureID) const
	{
		return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
			const auto& featureIDs = arg->getFeatureIDs();
			return std::find(featureIDs.begin(), featureIDs.end(), featureID) != featureIDs.end();
		});
	}

	std::string _name;
	ComponentsHolder_t _holder;
	#ifdef EDITOR
	bool _locked{false};
	#endif
};

}
