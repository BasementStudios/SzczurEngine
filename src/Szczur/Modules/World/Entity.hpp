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
    Entity(Scene& scene, const EntityID_t id);

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
    EntityID_t getID() const;

    ///
    bool addComponent(HashedID hid) const;

    ///
    bool hasComponent(HashedID hid) const;

    ///
    bool removeComponent(HashedID hid) const;

    ///
    bool addTag(HashedID hid) const;

    ///
    bool hasTag(HashedID hid) const;

    ///
    bool destroy() const;

    ///
    bool isValid() const;

private:

    Scene& _scene;
    const EntityID_t _id;

};

}
