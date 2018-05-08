#pragma once

#include <memory>
#include <vector>

#include "Scene.hpp"

namespace rat {
    class SceneManager {
    public:
        using ScenesHolder_t = std::vector<std::unique_ptr<Scene>>;

        ///
        SceneManager() = default;
        SceneManager(const SceneManager&) = delete;
        SceneManager& operator=(const SceneManager&) = delete;

        ///
        Scene* addScene();

	    /// 
        bool removeScene(size_t id);

        ///
        void removeAllScenes();

        ///
        Scene* getScene(size_t id) const;

        ///
        ScenesHolder_t& getScenes();

        ///
        const ScenesHolder_t& getScenes() const;

        ///
        bool hasScene(size_t id) const;

        ///
        Scene* getCurrentScene() const;

        ///
        size_t getCurrentSceneID() const;

        ///
        bool isCurrentSceneValid() const;

        ///
        void loadFromFile(const std::string& filepath);

        ///
        void saveToFile(const std::string& filepath) const;

    private:
        ScenesHolder_t _holder;

        size_t _currentSceneID{0u};

        ///
        typename ScenesHolder_t::iterator _find(size_t id);

        ///
        typename ScenesHolder_t::const_iterator _find(size_t id) const;
    };
}