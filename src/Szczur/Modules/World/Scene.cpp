#include "SceneManager.hpp"

#include <functional>

#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Utility/SFML3D/Drawable.hpp"
#include "Szczur/Utility/SFML3D/Sprite.hpp"
#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/RectangleShape.hpp"

// Components
#include "Components/ScriptableComponent.hpp"
#include "Components/InputControllerComponent.hpp"

#include "Szczur/Modules/Input/Input.hpp"

#include "UniqueID.hpp"

namespace rat
{

Scene::Scene(SceneManager* parent)
	: _id { getUniqueID<Scene>() }
	, _name { "unnamed_" + std::to_string(_id) }
	, _parent { parent }
{
	_collectingHolder.emplace("background", EntitiesHolder_t{});
	_collectingHolder.emplace("foreground", EntitiesHolder_t{});
	_collectingHolder.emplace("path", EntitiesHolder_t{});
	_collectingHolder.emplace("single", EntitiesHolder_t{});

	for (auto& holder : getAllEntities())
	{
		holder.second.reserve(100);
	}

	_spriteDisplayDataHolder.reserve(100);
	_armatureDisplayDataHolder.reserve(100);
}

void Scene::update(float deltaTime)
{
	auto& input = detail::globalPtr<Input>->getManager();
	auto* player = getEntity(getPlayerID());
	for (auto& holder : getAllEntities())
	{
		for (auto& entity : holder.second)
		{
			entity->update(deltaTime);
			if(auto* comp = entity->getComponentAs<ScriptableComponent>(); comp != nullptr) {
				comp->update(deltaTime);
				if(auto* comp = entity->getComponentAs<InputControllerComponent>(); comp != nullptr)
					comp->update(input, deltaTime);
				if(input.isReleased(Keyboard::LShift))
					if(auto* comp = entity->getComponentAs<InteractableComponent>(); comp != nullptr)
						if(comp->checkForInteraction(player->getPosition()))
							comp->callback();
			}
			if (auto* comp = entity->getComponentAs<TraceComponent>(); comp != nullptr) {
				comp->update(deltaTime);
			}
			if(auto* comp = entity->getComponentAs<CameraComponent>(); comp != nullptr) {
				if(comp->getStickToPlayer()) {
					auto curPos = entity->getPosition();
					curPos.x = player->getPosition().x;
					entity->setPosition(curPos);
				}
			}
			if(auto* comp = entity->getComponentAs<TriggerComponent>(); comp != nullptr) {
				if(input.isReleased(Keyboard::LShift)) {
					if(comp->checkForTrigger(player->getPosition())) {
						if(comp->type == TriggerComponent::ChangeScene) {
							getScenes()->setCurrentScene(comp->sceneId);
							auto* scene = getScenes()->getCurrentScene();
							for(auto& it : scene->getEntrances()) {
								if(it.ID == comp->entranceId) {
									scene->getEntity(scene->getPlayerID())->setPosition(it.position);
								}
							}
						}
					}
				}
			}
		}
	}
}

void Scene::render()
{
	for (auto& holder : getAllEntities())
	{
		for (auto& entity : holder.second)
		{
			entity->render();
		}
	}
}

size_t Scene::getID() const
{
	return _id;
}

void Scene::setName(const std::string& name)
{
	_name = name;
}

const std::string& Scene::getName() const
{
	return _name;
}

SceneManager* Scene::getScenes()
{
	return _parent;
}

const SceneManager* Scene::getScenes() const
{
	return _parent;
}

Entity* Scene::addEntity(const std::string& group)
{
	return getEntities(group).emplace_back(std::make_unique<Entity>(this, group)).get();
}

Entity* Scene::duplicateEntity(size_t id)
{
	if (auto ptr = getEntity(id); ptr != nullptr)
	{
		return getEntities(ptr->getGroup()).emplace_back(std::make_unique<Entity>(*ptr)).get();
	}

	return nullptr;
}

bool Scene::removeEntity(const std::string& group, size_t id)
{
	if (auto it = _find(group, id); it != getEntities(group).end())
	{
		getEntities(group).erase(it);

		return true;
	}

	return false;
}

bool Scene::removeEntity(size_t id)
{
	if (auto it = _find("single", id); it != getEntities("single").end())
	{
		getEntities("single").erase(it);

		return true;
	}

	if (auto it = _find("path", id); it != getEntities("path").end())
	{
		getEntities("path").erase(it);

		return true;
	}

	if (auto it = _find("foreground", id); it != getEntities("foreground").end())
	{
		getEntities("foreground").erase(it);

		return true;
	}

	if (auto it = _find("background", id); it != getEntities("background").end())
	{
		getEntities("background").erase(it);

		return true;
	}

	return false;
}

void Scene::removeAllEntities(const std::string& group)
{
	getEntities(group).clear();
}

void Scene::removeAllEntities()
{
	for (auto& holder : getAllEntities())
	{
		holder.second.clear();
	}
}

Entity* Scene::getEntity(size_t id)
{
	if (auto it = _find("single", id); it != getEntities("single").end())
	{
		return it->get();
	}

	if (auto it = _find("path", id); it != getEntities("path").end())
	{
		return it->get();
	}

	if (auto it = _find("foreground", id); it != getEntities("foreground").end())
	{
		return it->get();
	}

	if (auto it = _find("background", id); it != getEntities("background").end())
	{
		return it->get();
	}

	return nullptr;
}

const Entity* Scene::getEntity(size_t id) const
{
	if (auto it = _find("single", id); it != getEntities("single").end())
	{
		return it->get();
	}

	if (auto it = _find("path", id); it != getEntities("path").end())
	{
		return it->get();
	}

	if (auto it = _find("foreground", id); it != getEntities("foreground").end())
	{
		return it->get();
	}

	if (auto it = _find("background", id); it != getEntities("background").end())
	{
		return it->get();
	}

	return nullptr;
}

Entity* Scene::getEntity(const std::string& group, size_t id)
{
	if (auto it = _find(group, id); it != getEntities(group).end())
	{
		return it->get();
	}

	return nullptr;
}

const Entity* Scene::getEntity(const std::string& group, size_t id) const
{
	if (auto it = _find(group, id); it != getEntities(group).end())
	{
		return it->get();
	}

	return nullptr;
}

bool Scene::hasEntity(size_t id)
{
	if (_find("single", id) != getEntities("single").end())
	{
		return true;
	}

	if (_find("path", id) != getEntities("path").end())
	{
		return true;
	}

	if (_find("foreground", id) != getEntities("foreground").end())
	{
		return true;
	}

	if (_find("background", id) != getEntities("background").end())
	{
		return true;
	}

	return false;
}

bool Scene::hasEntity(const std::string& group, size_t id)
{
	return _find(group, id) != getEntities(group).end();
}

Scene::EntrancesHolder_t& Scene::getEntrances() {
	return _entrancesHolder;
}

Scene::EntitiesHolder_t& Scene::getEntities(const std::string& group)
{
	return getAllEntities().at(group);
}

const Scene::EntitiesHolder_t& Scene::getEntities(const std::string& group) const
{
	return getAllEntities().at(group);
}

Scene::CollectingHolder_t& Scene::getAllEntities()
{
	return _collectingHolder;
}

const Scene::CollectingHolder_t& Scene::getAllEntities() const
{
	return _collectingHolder;
}

Scene::SpriteDisplayDataHolder_t& Scene::getSpriteDisplayDataHolder()
{
	return _spriteDisplayDataHolder;
}

const Scene::SpriteDisplayDataHolder_t& Scene::getSpriteDisplayDataHolder() const
{
	return _spriteDisplayDataHolder;
}

Scene::ArmatureDisplayDataHolder_t& Scene::getArmatureDisplayDataHolder()
{
	return _armatureDisplayDataHolder;
}

const Scene::ArmatureDisplayDataHolder_t& Scene::getArmatureDisplayDataHolder() const
{
	return _armatureDisplayDataHolder;
}

void Scene::setPlayerID(size_t id)
{
	_playerID = id;
}

size_t Scene::getPlayerID() const
{
	return _playerID;
}

void Scene::loadFromConfig(const Json& config)
{
	_id = config["id"];
	_name = config["name"].get<std::string>();
	_playerID = config["player"];

	// size_t maxId = 0u;
	// for(auto& obj : config["entrances"]) {
	// 	if(obj["id"].get<size_t>() > maxId) {
	// 		maxId = obj["id"].get<size_t>();
	// 	}
	// 	_entrancesHolder.push_back(
	// 		Entrance{
	// 			obj["id"].get<size_t>(),
	// 			obj["name"].get<std::string>(),
	// 			glm::vec3{obj["position"]["x"].get<float>(), obj["position"]["y"].get<float>(), obj["position"]["z"].get<float>()}
	// 		}
	// 	);
	// }
	//
	// trySettingInitialUniqueID<Entrance>(maxId);

	const Json& groups = config["groups"];

	for (auto it = groups.begin(); it != groups.end(); ++it)
	{
		for (const Json& current : it.value())
		{
			addEntity(it.key())->loadFromConfig(current);
		}
	}

	trySettingInitialUniqueID<Scene>(_id);
}

void Scene::saveToConfig(Json& config) const
{
	config["id"] = getID();
	config["name"] = getName();
	config["player"] = getPlayerID();

	Json& entrances = config["entrances"] = Json::array();
	for(auto& it : _entrancesHolder) {
		entrances.push_back(Json::object());
		Json& current = entrances.back();

		current["name"] = it.name;
		current["id"] = it.ID;
		current["position"]["x"] = it.position.x;
		current["position"]["y"] = it.position.y;
		current["position"]["z"] = it.position.z;
	}

	Json& groups = config["groups"] = Json::object();

	for (auto& holder : getAllEntities())
	{
		Json& group = groups[holder.first] = Json::array();

		for (auto& entity : holder.second)
		{
			group.push_back(Json::object());
			Json& current = group.back();

			entity->saveToConfig(current);
		}
	}
}

void Scene::updateIDs()
{
	_id = getUniqueID<Scene>();

	for (auto& group : _collectingHolder)
	{
		for (auto& entity : group.second)
		{
			entity->updateIDs();
		}
	}
}

typename Scene::EntitiesHolder_t::iterator Scene::_find(const std::string& group, size_t id)
{
	auto& subHolder = getEntities(group);

	return std::find_if(subHolder.begin(), subHolder.end(), [=](const auto& arg) {
		return arg->getID() == id;
	});
}

typename Scene::EntitiesHolder_t::const_iterator Scene::_find(const std::string& group, size_t id) const
{
	const auto& subHolder = getEntities(group);

	return std::find_if(subHolder.begin(), subHolder.end(), [=](const auto& arg) {
		return arg->getID() == id;
	});
}

}
