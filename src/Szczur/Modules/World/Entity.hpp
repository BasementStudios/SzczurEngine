#pragma once

#include <entt/entt.hpp>

#include "Config.hpp"

namespace rat::wrd
{

// fwd decl
class Scene;

/// Practical wrapper around entity id, use 'as is', do not store anywhere
class Entity
{
public:

    ///
    Entity(Scene& scene, Registry_t& registry, const EntityID_t id);

    ///
    Entity(const Entity&) = delete;

    ///
    Entity& operator = (const Entity&) = delete;

    ///
    Entity(Entity&&) = delete;

    ///
    Entity& operator = (Entity&&) = delete;

    ///
    ~Entity() = default;

    ///
    Scene& getScene() const;

    ///
    Registry_t& getRegistry() const;

    ///
    EntityID_t getID() const;

    ///
    void addComponent(HashedID hid) const;

    ///
    void removeComponent(HashedID hid) const;

    ///
    Entity clone() const; // TODO how to do it

    ///
    bool isValid() const;

private:

    Scene& _scene;
    Registry_t& _registry;
    const EntityID_t _id;

};

}
