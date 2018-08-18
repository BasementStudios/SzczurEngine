#pragma once

#include <entt/entt.hpp>

#include "ComponentRegistry.hpp"

namespace rat::wrd
{

class Scene;

class WorldManager
{
public:

    using ScenesHolder_t      = std::vector<std::unique_ptr<Scene>>;
    using ComponentRegistry_t = ComponentRegistry;

    ///
    WorldManager();

    ///
    WorldManager(const WorldManager&) = delete;

    ///
    WorldManager& operator = (const WorldManager&) = delete;

    ///
    WorldManager(WorldManager&&) = default;

    ///
    WorldManager& operator = (WorldManager&&) = default;

    ///
    ~WorldManager() = default;

    ///
    void reset();

    ///
    Scene* addScene(const Name_t& name = "anonymous_scene");

    ///
    bool hasScene(ID_t id);

    ///
    bool hasScene(const Name_t& name);

    ///
    bool removeScene(ID_t id);

    ///
    bool removeScene(const Name_t& name);

    ///
    Scene* getScene(ID_t id) const;

    ///
    Scene* getScene(const Name_t& name) const;

    ///
    bool setCurrentScene(ID_t id);

    ///
    bool setCurrentScene(const Name_t& name);

    ///
    Scene* getCurrentScene() const;

    ///
    ID_t getCurrentSceneID() const;

    ///
    bool isCurrentSceneValid() const;

    ///
    ComponentRegistry_t& getComponentRegistry();

    ///
    const ComponentRegistry_t& getComponentRegistry() const;

private:

    ///
    typename ScenesHolder_t::iterator _find(ID_t id);

    ///
    typename ScenesHolder_t::const_iterator _find(ID_t id) const;

    ///
    typename ScenesHolder_t::iterator _find(const Name_t& name);

    ///
    typename ScenesHolder_t::const_iterator _find(const Name_t& name) const;

    ScenesHolder_t _scenes;
    ID_t _currentSceneID;
    ComponentRegistry_t _componentRegistry;

};

}
