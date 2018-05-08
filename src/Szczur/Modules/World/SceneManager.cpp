#include "SceneManager.hpp"

namespace rat {
    Scene* SceneManager::addScene()
    {
        return _holder.emplace_back(std::make_unique<Scene>()).get();
    }

    bool SceneManager::removeScene(size_t id)
    {
        if (auto it = _find(id); it != _holder.end())
        {
            _holder.erase(it);

            if (_currentSceneID == id)
                _currentSceneID = 0;

            return true;
        }

        return false;
    }

    void SceneManager::removeAllScenes()
    {
        _holder.clear();

        _currentSceneID = 0;
    }

    Scene* SceneManager::getScene(size_t id) const
    {
        if (auto it = _find(id); it != _holder.end())
            return it->get();
        return nullptr;
    }

    SceneManager::ScenesHolder_t& SceneManager::getScenes()
    {
        return _holder;
    }

    const SceneManager::ScenesHolder_t& SceneManager::getScenes() const
    {
        return _holder;
    }

    bool SceneManager::hasScene(size_t id) const
    {
        return _find(id) != _holder.end();
    }

    Scene* SceneManager::getCurrentScene() const
    {
        return getScene(_currentSceneID);
    }

    size_t SceneManager::getCurrentSceneID() const
    {
        return _currentSceneID;
    }

    bool SceneManager::isCurrentSceneValid() const
    {
        return _currentSceneID != 0;
    }

    void SceneManager::loadFromFile(const std::string& filepath)
    {
        std::ifstream file{ filepath };
        Json config;

        file >> config;

        _currentSceneID = config["currentSceneID"];

        const Json& scenes = config["scenes"];

        for (auto& current : scenes)
        {
            addScene()->loadFromConfig(current);
        }
    }

    void SceneManager::saveToFile(const std::string& filepath) const
    {
        std::ofstream file{ filepath };
        Json config;

        config["currentSceneID"] = getCurrentSceneID();
        Json& scenes = config["scenes"] = Json::array();

        for (auto& scene : _holder)
        {
            scenes.push_back(Json::object());
            Json& current = scenes.back();

            scene->saveToConfig(current);
        }

        file << std::setw(4) << config << std::endl;
    }

    typename SceneManager::ScenesHolder_t::iterator SceneManager::_find(size_t id)
    {
        return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
            return arg->getID() == id;
        });
    }

    typename SceneManager::ScenesHolder_t::const_iterator SceneManager::_find(size_t id) const
    {
        return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
            return arg->getID() == id;
        });
    }
}