#pragma once

#include <entt/entt.hpp>

#include <functional>

#include "Entity.hpp"

namespace rat::wrd
{

// fwd decl
class WorldManager;

class Scene
{
public:

    using WorldManager_t = WorldManager;

    ///
    Scene(WorldManager& manager, ID_t id, NameView_t name);

    ///
    Scene(const Scene&) = delete;

    ///
    Scene& operator = (const Scene&) = delete;

    ///
    Scene(Scene&&) = default;

    ///
    Scene& operator = (Scene&&) = default;

    ///
    ~Scene() = default;

    ///
    WorldManager_t& getManager() const;

    ///
    ID_t getID() const;

    ///
    void setName(const Name_t& name);

    ///
    const Name_t& getName() const;

    ///
    Registry_t& getRegistry();

    ///
    const Registry_t& getRegistry() const;

    ///
    Entity addEntity();

    ///
    bool hasEntity(EntityID_t id) const;

    ///
    bool removeEntity(EntityID_t id);

    ///
    Entity getEntity(EntityID_t id);

    ///
    template <typename Callable>
    void forEach(Callable&& callable);

private:


    WorldManager_t& _manager;
    ID_t _id;
    Name_t _name;
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
            std::invoke(std::forward<Callable>(callable), Entity{ scene, registry, id }, std::forward<decltype(components)>(components)...);
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
void Scene::forEach(Callable&& callable)
{
    using Traits_t = detail::FunctionTraits<Callable>;

    detail::Caller<typename Traits_t::IndexSeq_t>{ *this, _registry }(std::forward<Callable>(callable));
}

}
