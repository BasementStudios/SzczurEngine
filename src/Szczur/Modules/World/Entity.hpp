#pragma once

#include <unordered_map>

#include "Szczur/Utility/SFML3D/Transformable.hpp"

#include "Components.hpp"

#include <Szczur/Modules/Script/Script.hpp>

#include <Szczur/Utility/SFML3D/RenderTarget.hpp>

namespace rat
{

// FWD
class Scene;
class ScenesManager;

class Entity : public sf3d::Transformable
{
public:

	using ComponentsHolder_t = std::vector<std::unique_ptr<Component>>;

	///
	Entity(Scene* parent, const std::string& group);

	///
	Entity(const Entity& rhs);

	///
	Entity& operator = (const Entity& rhs);

	///
	Entity(Entity&&) = delete;

	///
	Entity& operator = (Entity&&) = delete;

	///
	virtual ~Entity() = default;

	///
	void update(ScenesManager& scenes, float deltaTime);

	///
	void render(sf3d::RenderTarget& canvas);

	///
	size_t getID() const;

	///
	void setGroup(const std::string& group);

	///
	const std::string& getGroup() const;

	///
	void setName(const std::string& name);

	///
	const std::string& getName() const;

	///
	Scene* getScene();

	///
	const Scene* getScene() const;

	///
	Component* addComponent(std::unique_ptr<Component> component);

	///
	Component* addComponent(Hash64_t componentID);

	///
	template <typename T>
	Component* addComponent()
	{
		return addComponent(ComponentTraits::getIdentifierFromType<T>());
	}

	///
	bool removeComponent(Hash64_t componentID);

	///
	void removeAllComponents();

	///
	template <typename T>
	bool removeComponent()
	{
		return removeComponent(ComponentTraits::getIdentifierFromType<T>());
	}

	///
	Component* getComponent(Hash64_t componentID) const;

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
	bool hasComponent(Hash64_t componentID) const;

	///
	template <typename T>
	bool hasComponent() const
	{
		return hasComponent(ComponentTraits::getIdentifierFromType<T>());
	}

	///
	ComponentsHolder_t& getComponents();

	///
	const ComponentsHolder_t& getComponents() const;

	///
	virtual void loadFromConfig(Json& config);

	///
	virtual void saveToConfig(Json& config) const;

	///
	static void initScript(Script& script);

	///
	void updateIDs();

private:

	///
	typename ComponentsHolder_t::iterator _findByComponentID(size_t id);

	///
	typename ComponentsHolder_t::const_iterator _findByComponentID(size_t id) const;

	///
	typename ComponentsHolder_t::iterator _findByFeature(Component::Feature_e feature);

	///
	typename ComponentsHolder_t::const_iterator _findByFeature(Component::Feature_e feature) const;

	///
	void _setScriptDataObject(std::string key, sol::stack_object value);

	///
	sol::object _getScriptDataObject(const std::string& key);

// Script

	std::unordered_map<std::string, sol::object> _scriptData;

// Main

	size_t _id;
	std::string _group;
	std::string _name;
	Scene* _parent;
	ComponentsHolder_t _holder;
};

}
