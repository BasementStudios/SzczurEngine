#include "WorldManager.hpp"

#include "Scene.hpp"

namespace rat::world
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
	static Hash_t uniqueID = 0;

	Scene* scene = _scenes.emplace_back(std::make_unique<Scene>(*this, ++uniqueID, name)).get();

	// TODO add default entities or better make player "global" entity

	return scene;
}

bool WorldManager::hasScene(HashedID hid)
{
	return _find(hid) != _scenes.end();
}

bool WorldManager::removeScene(HashedID hid)
{
	const auto it = _find(hid);

	if (it != _scenes.end())
	{
		_scenes.erase(it);

		if (_currentSceneID == hid.hash)
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

Scene* WorldManager::getScene(HashedID hid) const
{
	auto it = _find(hid);

	return it == _scenes.end() ? nullptr : it->get();
}

bool WorldManager::setCurrentScene(HashedID hid)
{
	if (hasScene(hid))
	{
		_currentSceneID = hid.hash;

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

Hash_t WorldManager::getCurrentSceneID() const
{
	return _currentSceneID;
}

bool WorldManager::isCurrentSceneValid() const
{
	return _currentSceneID != 0;
}

typename WorldManager::ScenesHolder_t::iterator WorldManager::_find(HashedID hid)
{
	return std::find_if(_scenes.begin(), _scenes.end(), [=](const auto& scenePtr) {
		return scenePtr->getID() == hid.hash;
	});
}

typename WorldManager::ScenesHolder_t::const_iterator WorldManager::_find(HashedID hid) const
{
	return std::find_if(_scenes.begin(), _scenes.end(), [=](const auto& scenePtr) {
		return scenePtr->getID() == hid.hash;
	});
}

}
