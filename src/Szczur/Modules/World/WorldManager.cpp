#include "WorldManager.hpp"

#include "Scene.hpp"

namespace rat::wrd
{

WorldManager::WorldManager()
    : _scenes {}
    , _currentSceneID { 0 }
{

}

void WorldManager::reset()
{
    _scenes.clear();

    _currentSceneID = 0;
}

Scene* WorldManager::addScene(const Name_t& name)
{
    static ID_t id = 0;

    Scene* scene = _scenes.emplace_back(std::make_unique<Scene>(*this, ++id, name)).get();

    // TODO add default entities or better make player global entity

    return scene;
}

bool WorldManager::hasScene(ID_t id)
{
    return _find(id) != _scenes.end();
}

bool WorldManager::hasScene(const Name_t& name)
{
    return _find(name) != _scenes.end();
}

bool WorldManager::removeScene(ID_t id)
{
    const auto it = _find(id);

    if (it != _scenes.end())
    {
        _scenes.erase(it);

        if (_currentSceneID == id)
		{
			if (_scenes.empty())
            {
                _currentSceneID = addScene()->getID();
            }
			else
            {
                _currentSceneID = _scenes.front()->getID();
            }
		}

        return true;
    }
    else
    {
        return false;
    }
}

bool WorldManager::removeScene(const Name_t& name)
{
    const auto it = _find(name);

    if (it != _scenes.end())
    {
        const ID_t id = it->get()->getID();

        _scenes.erase(it);

        if (_currentSceneID == id)
		{
			if (_scenes.empty())
            {
                _currentSceneID = addScene()->getID();
            }
			else
            {
                _currentSceneID = _scenes.front()->getID();
            }
		}

        return true;
    }
    else
    {
        return false;
    }
}

Scene* WorldManager::getScene(ID_t id) const
{
    auto it = _find(id);

    return it == _scenes.end() ? nullptr : it->get();
}

Scene* WorldManager::getScene(const Name_t& name) const
{
    auto it = _find(name);

    return it == _scenes.end() ? nullptr : it->get();
}

bool WorldManager::setCurrentScene(ID_t id)
{
    if (hasScene(id))
    {
        _currentSceneID = id;

        return true;
    }
    else
    {
        return false;
    }
}

bool WorldManager::setCurrentScene(const Name_t& name)
{
    Scene* scene = getScene(name);

    if (scene)
    {
        _currentSceneID = scene->getID();

        return true;
    }
    else
    {
        return false;
    }
}

Scene* WorldManager::getCurrentScene() const
{
    if (_currentSceneID == 0)
    {
        return nullptr;
    }
    else
    {
        return getScene(_currentSceneID);
    }
}

ID_t WorldManager::getCurrentSceneID() const
{
    return _currentSceneID;
}

bool WorldManager::isCurrentSceneValid() const
{
    return _currentSceneID != 0;
}

WorldManager::ComponentRegistry_t& WorldManager::getComponentRegistry()
{
    return _componentRegistry;
}

const WorldManager::ComponentRegistry_t& WorldManager::getComponentRegistry() const
{
    return _componentRegistry;
}

typename WorldManager::ScenesHolder_t::iterator WorldManager::_find(ID_t id)
{
	return std::find_if(_scenes.begin(), _scenes.end(), [=](const auto& scenePtr) {
		return scenePtr->getID() == id;
	});
}

typename WorldManager::ScenesHolder_t::const_iterator WorldManager::_find(ID_t id) const
{
	return std::find_if(_scenes.begin(), _scenes.end(), [=](const auto& scenePtr) {
		return scenePtr->getID() == id;
	});
}

typename WorldManager::ScenesHolder_t::iterator WorldManager::_find(const Name_t& name)
{
	return std::find_if(_scenes.begin(), _scenes.end(), [&](const auto& scenePtr) {
		return scenePtr->getName() == name;
	});
}

typename WorldManager::ScenesHolder_t::const_iterator WorldManager::_find(const Name_t& name) const
{
	return std::find_if(_scenes.begin(), _scenes.end(), [&](const auto& scenePtr) {
		return scenePtr->getName() == name;
	});
}

}
