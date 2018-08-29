#pragma once

#include <functional>

#include <entt/entt.hpp>

#include <nlohmann/json.hpp>

#include "Entity.hpp"

namespace rat::world
{

// fwd decl
class Scene;

class EntityManager
{
public:

	/* Basics */

	/// Basic ctor
	EntityManager(Scene& scene);

	/// Deleted copy-ctor
	EntityManager(const EntityManager&) = delete;

	/// Deleted copy-assign
	EntityManager& operator = (const EntityManager&) = delete;

	/// Defaulted move-ctor
	EntityManager(EntityManager&&) = default;

	/// Defaulted move-assign
	EntityManager& operator = (EntityManager&&) = default;

	/// Defaulted dtor
	~EntityManager() = default;

	/* Getters */

	/// Returns reference to scene which holds this manager
	Scene& getScene() const;

	/// Returns reference to entt's registry
	Registry_t& getRegistry();

	/// Returns const reference to entt's registry
	const Registry_t& getRegistry() const;

	/* Entity */

	/// Creates new entity and returns non-owning wrapper for it
	Entity addEntity();

	/// Checks whether entity exists
	bool hasEntity(EntityID_t id) const;

	/// Removes component from entity, returns false if entity does not exits
	bool removeEntity(EntityID_t id);

	/// Returns non-owning wrapper for entity
	Entity getEntity(EntityID_t id);

	/* Component */

	/// Adds component to entity, returns false if entity already has demanded component
	bool addComponent(EntityID_t id, HashedID hid);

	/// Checks whether entity has component
	bool hasComponent(EntityID_t id, HashedID hid);

	/// Removes component from entity, returns false if entity does not have demanded component
	bool removeComponent(EntityID_t id, HashedID hid);

	/* Utility */

	/// Resets whole manager, deletes everything
	void reset();

	/// Invokes callable for each entity with components required by callable
	template <typename Callable>
	void forEach(Callable&& callable);

	/// Saves each entity and all corresponding components that match Components... to Json config
	nlohmann::json saveToConfig();

	/// Loads entities with their components from Json config
	void loadFromConfig(const nlohmann::json& config);

private:

	Scene& _scene;
	Registry_t _registry;

};

namespace detail
{

template <typename T>
struct FunctionTraits : public FunctionTraits<decltype(&T::operator())> {};

template <typename Class, typename Ret, typename... Args>
struct FunctionTraits<Ret(Class::*)(const Entity&, Args...) const>
{
	template <size_t N>
	struct ArgHelper
	{
		using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
	};

	static constexpr auto arity = sizeof...(Args);

	template <size_t N>
	using Arg_t      = typename ArgHelper<N>::type;
	using IndexSeq_t = std::make_index_sequence<arity>;

};

template <typename T>
using PureType_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename>
struct Caller;

template <size_t... Is>
struct Caller<std::index_sequence<Is...>>
{
	template <typename... Ts, typename Callable>
	void call(Callable&& callable)
	{
		registry.view<Ts...>().each([&](const EntityID_t id, auto&&... components) {
			std::invoke(std::forward<Callable>(callable), Entity{ scene, id }, std::forward<decltype(components)>(components)...);
		});
	}

	template <typename Callable>
	void operator () (Callable&& callable)
	{
		using Traits_t = FunctionTraits<Callable>;

		call<PureType_t<typename Traits_t::template Arg_t<Is>>...>(std::forward<Callable>(callable));
	}

	Scene& scene;
	Registry_t& registry;

};

}

template <typename Callable>
void EntityManager::forEach(Callable&& callable)
{
	using Traits_t = detail::FunctionTraits<Callable>;

	if constexpr (Traits_t::arity == 0u)
	{
		_registry.each([&](const EntityID_t id) {
			std::invoke(std::forward<Callable>(callable), Entity{ _scene, id });
		});
	}
	else
	{
		detail::Caller<typename Traits_t::IndexSeq_t>{ _scene, _registry }(std::forward<Callable>(callable));
	}
}

}
