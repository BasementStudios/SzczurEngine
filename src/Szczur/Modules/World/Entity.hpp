#pragma once

#include "Szczur/Utility/SFML3D/Transformable.hpp"

#include "Components.hpp"

namespace rat
{

// FWD
class Scene;

class Entity : public sf3d::Transformable
{
public:

	using ComponentsHolder_t = std::vector<std::unique_ptr<Component>>;

	///
	Entity(Scene* parent);

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
	void update(float deltaTime);

	///
	void render();

	///
	size_t getID() const;

	///
	void setName(const std::string& name);

	///
	const std::string& getName() const;

	///
	Scene* getScene();

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
	Component* addComponent(Hash64_t componentID)
	{
		return addComponent(ComponentTraits::createFromComponentID(this, componentID));
	}

	///
	template <typename T>
	Component* addComponent()
	{
		return addComponent(ComponentTraits::getIdentifierFromType<T>());
	}

	///
	void removeAllComponents();

	///
	bool removeComponent(Hash64_t componentID)
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
		return removeComponent(ComponentTraits::getIdentifierFromType<T>());
	}

	///
	Component* getComponent(Hash64_t componentID) const
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
		return getComponent(ComponentTraits::getIdentifierFromType<T>());
	}

	///
	template <typename T>
	T* getComponentAs() const
	{
		return static_cast<T*>(getComponent(ComponentTraits::getIdentifierFromType<T>()));
	}

	///
	template <typename T>
	T* getFeature()
	{
		auto feature = ComponentTraits::getFeatureFromType<T>();

		if (auto it = _findByFeature(feature); it != _holder.end())
		{
			return static_cast<T*>(it->get()->getFeature(feature));
		}

		return nullptr;
	}

	///
	template <typename T>
	const T* getFeature() const
	{
		auto feature = ComponentTraits::getFeatureFromType<T>();

		if (auto it = _findByFeature(feature); it != _holder.end())
		{
			return static_cast<const T*>(it->get()->getFeature(feature));
		}

		return nullptr;
	}

	///
	bool hasComponent(Hash64_t componentID) const
	{
		return _findByComponentID(componentID) != _holder.end();
	}

	///
	template <typename T>
	bool hasComponent() const
	{
		return hasComponent(ComponentTraits::getIdentifierFromType<T>());
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
	typename ComponentsHolder_t::iterator _findByFeature(Component::Feature_e feature);

	///
	typename ComponentsHolder_t::const_iterator _findByFeature(Component::Feature_e feature) const;

	size_t _id;
	std::string _name;
	Scene* _parent;
	ComponentsHolder_t _holder;

};

}
