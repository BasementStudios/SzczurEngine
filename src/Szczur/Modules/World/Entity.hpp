#pragma once

#include <entt/entt.hpp>

#include "Config.hpp"

namespace rat::wrd
{

// fwd decl
class EntityManager;
class Scene;

/// Practical wrapper around entity id, use 'as is', do not store anywhere
class Entity
{
public:

    ///
    Entity(Scene& scene, EntityID_t id);

    ///
    Entity(const Entity&) = default;

    ///
    Entity& operator = (const Entity&) = default;

    ///
    Entity(Entity&&) = default;

    ///
    Entity& operator = (Entity&&) = default;

    ///
    ~Entity() = default;

    ///
    Scene& getScene() const;

    ///
    EntityManager& getEntityManager() const;

    ///
    EntityID_t getID() const;

    ///
    bool addComponent(HashedID hid) const;

    ///
    bool hasComponent(HashedID hid) const;

    ///
    bool removeComponent(HashedID hid) const;

    ///
    bool destroy() const;

    ///
    bool isValid() const;

private:

    Scene& _scene;
    EntityID_t _id;

};

}
