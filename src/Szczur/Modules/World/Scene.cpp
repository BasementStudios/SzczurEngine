#include "ScenesManager.hpp"

#include <string>
#include <functional> // find_if

#include <nlohmann/json.hpp>

#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Utility/SFML3D/Drawable.hpp"
#include "Szczur/Utility/SFML3D/Sprite.hpp"
#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/Geometry/RectangleShape.hpp"

// Components
#include "Components/ScriptableComponent.hpp"
#include "Components/CameraComponent.hpp"
#include "Components/PointLightComponent.hpp"

#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/World/World.hpp"

#include "UniqueID.hpp"

namespace rat
{

Scene::Scene(ScenesManager* parent)
	: _id { getUniqueID<Scene>() }
	, _name { "unnamed_" + std::to_string(_id) }
	, _parent { parent }
{
	_collectingHolder.emplace_back("background", EntitiesHolder_t{}); 
	_collectingHolder.emplace_back("single", EntitiesHolder_t{});
	_collectingHolder.emplace_back("path", EntitiesHolder_t{});
	_collectingHolder.emplace_back("entries", EntitiesHolder_t{}); 
	_collectingHolder.emplace_back("battles", EntitiesHolder_t{}); 
	_collectingHolder.emplace_back("foreground", EntitiesHolder_t{}); 

	for (auto& holder : getAllEntities())
	{
		holder.second.reserve(100);
	}

	_spriteDisplayDataHolder.reserve(100);
}

void Scene::update(float deltaTime)
{
	_parent->getTextureDataHolder().loadAll();
	for (auto& holder : getAllEntities())
	{
		for (auto& entity : holder.second)
		{
			if (entity->exists())
			{
				entity->update(*getScenes(), deltaTime);
			}
			else
			{
				removeEntity(holder.first, entity->getID());
			}
		}
	}

	if (Entity* cameraEntity = getCamera()) {
		cameraEntity->getComponentAs<CameraComponent>()->updateCamera();
	}
}

void Scene::render(sf3d::RenderTarget& target) 
{
	sf3d::RenderLayer& layer = this->getScenes()->getHelperRenderLayer();

	// Register light components to shader
	layer.resetLightPoints();
	for (auto& holder : this->getAllEntities()) {
		for (auto& entity : holder.second) {
			if (PointLightComponent* component = entity->getComponentAs<PointLightComponent>()) {
				if (entity->isVisible()) {
					layer.registerLightPoint(component);
					component->setPosition(entity->getPosition());
				}
			}
		}
	}

	// Background 
	// glEnable(GL_DEPTH_TEST);
	// layer.clear({0.f, 0.f, 0.f, 0.f}, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto& entity : this->getEntities("background")) {
		entity->draw(layer);
	}
	// glDisable(GL_DEPTH_TEST);
	// target.draw(layer);
	
	// Path & single
	// glEnable(GL_DEPTH_TEST);
	// layer.clear({0.f, 0.f, 0.f, 0.f}, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto& entity : this->getEntities("path")) {
		entity->draw(layer);
	}
	for (auto& entity : this->getEntities("single")) {
		entity->draw(layer);
	}
	// glDisable(GL_DEPTH_TEST);
	// target.draw(layer);

	// Foreground 
	// glEnable(GL_DEPTH_TEST);
	// layer.clear({0.f, 0.f, 0.f, 0.f}, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto& entity : this->getEntities("foreground")) {
		entity->draw(layer);
	}
	// glDisable(GL_DEPTH_TEST);
	// target.draw(layer);
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

ScenesManager* Scene::getScenes()
{
	return _parent;
}

const ScenesManager* Scene::getScenes() const
{
	return _parent;
}

Entity* Scene::addEntity(const std::string& group)
{
	auto* entity = getEntities(group).emplace_back(std::make_unique<Entity>(this, group)).get();

	#ifdef EDITOR
	auto* base = entity->addComponent<BaseComponent>();
	if(group == "entries") {
		dynamic_cast<BaseComponent*>(base)->positionOnly(true);
	}
	#endif //EDITOR

	return entity;
}


Entity* Scene::addRawEntity(const std::string& group)
{
	auto* entity = getEntities(group).emplace_back(std::make_unique<Entity>(this, group)).get();

	return entity;
}

Entity* Scene::duplicateEntity(size_t id)
{
	if (auto ptr = getEntity(id); ptr != nullptr)
	{
		auto entity = getEntities(ptr->getGroup()).emplace_back(std::make_unique<Entity>(*ptr)).get();

		if (auto comp = entity->getComponentAs<ScriptableComponent>(); comp != nullptr)
		{
			comp->callInit();
		}

		return entity;
	}

	return nullptr;
}

void Scene::changeEntityGroup(Entity* entity, const std::string& group)
{
	auto& currentGroup = getEntities(entity->getGroup());
	auto& newGroup = getEntities(group);

	auto it = std::find_if(currentGroup.begin(), currentGroup.end(), [&] (const std::unique_ptr<Entity>& it) { return it->getID() == entity->getID(); });

	if (it != currentGroup.end())
	{
		entity->setGroup(group);

		std::unique_ptr<Entity> ptr;

		// nie wiem jak wyjąć czysty unique_ptr z iteratora ;/
		it->swap(ptr);

		newGroup.push_back(std::move(ptr));

		currentGroup.erase(it);
	}
}

bool Scene::removeEntity(const std::string& group, size_t id)
{
	if (auto it = _find(group, id); it != getEntities(group).end())
	{
		#ifdef EDITOR
		auto& objects = detail::globalPtr<World>->getLevelEditor().getObjectsList();
		if (it->get()->getID() == objects.getSelectedID())
		{
			objects.unselect();
		}
		#endif //EDITOR

		getEntities(group).erase(it);

		return true;
	}

	return false;
}

bool Scene::removeEntity(size_t id)
{
	bool deleted = false;

	deleted += removeEntity("single", id);
	deleted += removeEntity("path", id);
	deleted += removeEntity("foreground", id);
	deleted += removeEntity("background", id);
	deleted += removeEntity("entries", id);
	deleted += removeEntity("battles", id);

	return deleted;
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

	if (auto it = _find("entries", id); it != getEntities("entries").end())
	{
		return it->get();
	}

	if (auto it = _find("battles", id); it != getEntities("battles").end())
	{
		return it->get();
	}

	return nullptr;
}

Entity* Scene::getEntity(const std::string& name) {
	for(auto& group : _collectingHolder) {
		for(auto& entity : group.second) {
			if(entity->getName() == name) {
				return entity.get();
			}
		}
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

	if (auto it = _find("entries", id); it != getEntities("entries").end())
	{
		return it->get();
	}

	if (auto it = _find("battles", id); it != getEntities("battles").end())
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

	if (_find("entries", id) != getEntities("entries").end())
	{
		return true;
	}

	if (_find("battles", id) != getEntities("battles").end())
	{
		return true;
	}

	return false;
}

bool Scene::hasEntity(const std::string& group, size_t id)
{
	return _find(group, id) != getEntities(group).end();
}

Scene::EntitiesHolder_t& Scene::getEntities(const std::string& group) 
{ 
	for(auto& gr : getAllEntities()) { 
		if(gr.first == group) return gr.second; 
	} 
	return getAllEntities()[0].second; 
} 

const Scene::EntitiesHolder_t& Scene::getEntities(const std::string& group) const 
{ 
	for(auto& gr : getAllEntities()) { 
		if(gr.first == group) return gr.second; 
	} 
	return getAllEntities()[0].second; 
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

// Player
Entity* Scene::getPlayer()
{
	return _player;
}
const Entity* Scene::getPlayer() const
{
	return _player;
}
void Scene::setPlayer(Entity* player)
{
	_player = player;
}

// CurrentCamera
Entity* Scene::getCurrentCamera()
{
	return _currentCamera;
}
const Entity* Scene::getCurrentCamera() const
{
	return _currentCamera;
}
void Scene::setCurrentCamera(Entity* camera)
{
	_currentCamera = camera;
}

Entity* Scene::getCamera()
{
	if (!_currentCamera) {
		// @todo ! ja pierdole te kontenery
		for (auto& entity : getEntities("single")) {
			// @todo . Zrobić changeCamera i pozwolić na wiele kamer.
			if (entity->hasComponent<CameraComponent>()) {
				_currentCamera = entity.get();
			}
		}
	}
	return _currentCamera;
}

//170
void Scene::loadFromConfig(nlohmann::json& config, bool withNewID)
{
	_id = withNewID ? getUniqueID<Entity>() : config["id"].get<size_t>(); 
	_name = config["name"].get<std::string>();

	size_t maxId = 0u;
	
	trySettingInitialUniqueID<Entrance>(maxId);

	nlohmann::json& groups = config["groups"];

	for (auto it = groups.begin(); it != groups.end(); ++it)
	{
		for (nlohmann::json& current : it.value())
		{
			addRawEntity(it.key())->loadFromConfig(current);
		}
	}

	if (!config["player"].is_null()) {
		// @todo @warn There is already _player loaded in ScenesManager, so here probably is obcsle.
		_player = getEntity(config["player"].get<int>());
	}
	else {
		_player = nullptr;
	}

	trySettingInitialUniqueID<Scene>(_id);
}

void Scene::saveToConfig(nlohmann::json& config) const
{
	// Scene informations 
	config["id"] = getID();
	config["name"] = getName();
	
	// Save player entity ID 
	config["player"] = getPlayer()->getID();

	nlohmann::json& groups = config["groups"] = nlohmann::json::object();

	for (auto& holder : getAllEntities())
	{
		nlohmann::json& group = groups[holder.first] = nlohmann::json::array();

		for (auto& entity : holder.second)
		{
			group.push_back(nlohmann::json::object());
			nlohmann::json& current = group.back();

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

void Scene::initScript(Script& script) {
	auto object = script.newClass<Scene>("Scene", "World");

	object.set("getEntity", sol::resolve<Entity*(const std::string&)>(&Scene::getEntity));
	object.set("addEntity", [&](Scene* s, const std::string& group, const std::string& name){
		auto* entity = s->addEntity(group);
		entity->setName(name);
		return entity;
	});
	object.setOverload("cloneEntity",
		[&](Scene* s, const std::string& name) {
			return s->duplicateEntity(s->getEntity(name)->getID());
		},
		[&](Scene* s, Entity* e){
			return s->duplicateEntity(e->getID());
		}
	);
	object.setOverload("removeEntity", 
		[&](Scene* s, const std::string& name) {
			return s->removeEntity(s->getEntity(name)->getID());
		},
		[&](Scene* s, Entity* e){
			return s->removeEntity(e->getID());
		}
	);

	object.init();
}

}
