#include "SceneManager.hpp"

#include <nlohmann/json.hpp>

#include <fstream> // ofstream, ifstream
#include <memory> // make_unique
#include <algorithm> // find_if

#include "Szczur/Utility/Logger.hpp"
#include "Scene.hpp"

namespace rat
{

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
        {
            _currentSceneID = 0u;
        }

        return true;
    }

    return false;
}

void SceneManager::removeAllScenes()
{
    _holder.clear();

    _currentSceneID = 0u;
}

Scene* SceneManager::getScene(size_t id) const
{
    if (auto it = _find(id); it != _holder.end())
    {
        return it->get();
    }

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

bool SceneManager::setCurrentScene(size_t id)
{
    if (hasScene(id))
    {
        _currentSceneID = id;

        return true;
    }

    return false;
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
    return _currentSceneID != 0u;
}

void SceneManager::loadFromFile(const std::string& filepath)
{
    removeAllScenes();

    std::ifstream file{ filepath };
    nlohmann::json config;

    file >> config;
    
    _currentSceneID = config["currentSceneID"];

    nlohmann::json& scenes = config["scenes"];

    for (auto& current : scenes)
    {
        addScene()->loadFromConfig(current);
    }
}

void SceneManager::appendScenesFromFile(const std::string &filepath)
{
    std::ifstream file{ filepath };
    nlohmann::json config;

    file >> config;

    nlohmann::json& scenes = config["scenes"];

    for (auto& current : scenes)
    {
        addScene()->loadFromConfig(current, true);
    }
}

void SceneManager::saveToFile(const std::string& filepath) const
{
    std::ofstream file{ filepath };
    nlohmann::json config;

    config["currentSceneID"] = getCurrentSceneID();
    nlohmann::json& scenes = config["scenes"] = nlohmann::json::array();

    for (auto& scene : _holder)
    {
        scenes.push_back(nlohmann::json::object());
        nlohmann::json& current = scenes.back();

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
