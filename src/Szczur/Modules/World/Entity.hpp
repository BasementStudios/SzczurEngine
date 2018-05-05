#pragma once

#include "Szczur/Utility/SFML3D/Transformable.hpp"

#include "Components/Components.hpp"

namespace rat
{

// FWD
class Scene;

class Entity : public sf3d::Transformable
{
public:

	using ComponentsHolder_t = std::vector<std::unique_ptr<Component>>;

	///
	Entity(const Scene* parent);

	///
	Entity(const Entity& rhs);

	///
	Entity& operator = (const Entity& rhs);

	///
	Entity(Entity&&) = default;

	///
	Entity& operator = (Entity&&) = default;

	///
	virtual ~Entity() = default;

	///
	void update();

	///
	void render();

	///
	size_t getID() const;

	///
	void setName(const std::string& name);

	///
	const std::string& getName() const;

	///
	const Scene* getScene() const;

	///
	Component* addComponent(std::unique_ptr<Component> component)
	{
		if (auto ptr = getComponent(component->getComponentID()); ptr != nullptr)
		{
			LOG_WARNING("Entity ( ", getID(), " ) ", std::quoted(getName()), " already has ", ComponentTraits::getNameFromIdentifier(ptr->getComponentID()), " component, existing one returned");

			return ptr;
		}

		return _holder.emplace_back(std::move(component)).get();
	}

	///
	Component* addComponent(size_t componentID)
	{
		return addComponent(ComponentTraits::createFromComponentID(componentID));
	}

	///
	template <typename T>
	Component* addComponent()
	{
		return addComponent(typeID<T>());
	}

	///
	void removeAllComponents();

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
	bool hasComponent(size_t componentID) const
	{
		return _findByComponentID(componentID) != _holder.end();
	}

	///
	template <typename T>
	bool hasComponent() const
	{
		return hasComponent(typeID<T>());
	}

	///
	const ComponentsHolder_t& getComponents() const;

	///
	virtual void loadFromConfig(const Json& config);

	///
	virtual void saveToConfig(Json& config) const;

private:

	///
	static size_t _getUniqueID();

	///
	typename ComponentsHolder_t::iterator _findByComponentID(size_t id);

	///
	typename ComponentsHolder_t::const_iterator _findByComponentID(size_t id) const;

	///
	typename ComponentsHolder_t::iterator _findByFeatureID(size_t featureID);

	///
	typename ComponentsHolder_t::const_iterator _findByFeatureID(size_t featureID) const;

	size_t _id;
	std::string _name;
	const Scene* _parent;
	ComponentsHolder_t _holder;

};

}
