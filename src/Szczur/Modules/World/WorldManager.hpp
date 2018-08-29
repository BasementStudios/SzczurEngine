#pragma once

#include <entt/entt.hpp>

#include "Config.hpp"

namespace rat::world
{

// fwd decl
class Scene;

class WorldManager
{
public:

    using ScenesHolder_t = std::vector<std::unique_ptr<Scene>>;

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
    Scene* addScene(const Name_t& name);

    ///
    bool hasScene(HashedID hid);

    ///
    bool removeScene(HashedID hid);

    ///
    Scene* getScene(HashedID hid) const;

    ///
    bool setCurrentScene(HashedID hid);

    ///
    Scene* getCurrentScene() const;

    ///
    Hash_t getCurrentSceneID() const;

    ///
    bool isCurrentSceneValid() const;

private:

    ///
    typename ScenesHolder_t::iterator _find(HashedID hid);

    ///
    typename ScenesHolder_t::const_iterator _find(HashedID hid) const;

    ScenesHolder_t _scenes;
    Hash_t _currentSceneID;

};

}
