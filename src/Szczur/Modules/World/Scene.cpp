#include "ScenesManager.hpp"

#include <functional>

#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Utility/SFML3D/Drawable.hpp"
#include "Szczur/Utility/SFML3D/Sprite.hpp"
#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/RectangleShape.hpp"

// Components
#include "Components/ScriptableComponent.hpp"

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
	_collectingHolder.emplace_back("path", EntitiesHolder_t{}); 
	_collectingHolder.emplace_back("single", EntitiesHolder_t{}); 
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
			entity->update(*getScenes(), deltaTime);
		}
	}

	/*for(auto& ent : getEntities("single")) {
		if(auto* comp = ent->getComponentAs<CameraComponent>()) {
			auto* camera = detail::globalPtr<Camera>;
			sf3d::View view{camera->getView()};
			auto delta = ent->getPosition() - view.getCenter();
			auto deltaRotation = ent->getRotation() - view.getRotation();
			float smoothness{comp->getSmoothness()};
			if(smoothness != 0.f) {
				view.move(delta/smoothness);
				view.rotate(deltaRotation/smoothness);
			}
		}
	}*/
}

void Scene::render(sf3d::RenderTarget& canvas)
{
	for (auto& holder : getAllEntities())
	{
		for (auto& entity : holder.second)
		{
			entity->render(canvas);
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
		#ifdef EDITOR
		auto& objects = detail::globalPtr<World>->getLevelEditor().getObjectsList();
		if(it->get()->getID() == objects.getSelectedID()) {
			objects.unselect();			
		}
		#endif //EDITOR

		getEntities("single").erase(it);

		return true;
	}

	if (auto it = _find("path", id); it != getEntities("path").end())
	{
		#ifdef EDITOR
		auto& objects = detail::globalPtr<World>->getLevelEditor().getObjectsList();
		if(it->get()->getID() == objects.getSelectedID()) {
			objects.unselect();			
		}
		#endif //EDITOR

		getEntities("path").erase(it);

		return true;
	}

	if (auto it = _find("foreground", id); it != getEntities("foreground").end())
	{
		#ifdef EDITOR
		auto& objects = detail::globalPtr<World>->getLevelEditor().getObjectsList();
		if(it->get()->getID() == objects.getSelectedID()) {
			objects.unselect();			
		}
		#endif //EDITOR

		getEntities("foreground").erase(it);

		return true;
	}

	if (auto it = _find("background", id); it != getEntities("background").end())
	{
		#ifdef EDITOR
		auto& objects = detail::globalPtr<World>->getLevelEditor().getObjectsList();
		if(it->get()->getID() == objects.getSelectedID()) {
			objects.unselect();			
		}
		#endif //EDITOR

		getEntities("background").erase(it);

		return true;
	}

	if (auto it = _find("entries", id); it != getEntities("entries").end())
	{
		#ifdef EDITOR
		auto& objects = detail::globalPtr<World>->getLevelEditor().getObjectsList();
		if(it->get()->getID() == objects.getSelectedID()) {
			objects.unselect();			
		}
		#endif //EDITOR

		getEntities("entries").erase(it);

		return true;
	}

	if (auto it = _find("battles", id); it != getEntities("battles").end())
	{
		#ifdef EDITOR
		auto& objects = detail::globalPtr<World>->getLevelEditor().getObjectsList();
		if(it->get()->getID() == objects.getSelectedID()) {
			objects.unselect();			
		}
		#endif //EDITOR

		getEntities("battles").erase(it);

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

void Scene::setPlayerID(size_t id)
{
	_playerID = id;
	_player = getEntity(id);
}

size_t Scene::getPlayerID() const
{
	return _playerID;
}

Entity* Scene::getPlayer()
{
	return _player;
}

//170
void Scene::loadFromConfig(Json& config)
{
	_id = config["id"];
	_name = config["name"].get<std::string>();

	size_t maxId = 0u;
	
	trySettingInitialUniqueID<Entrance>(maxId);

	Json& groups = config["groups"];

	for (auto it = groups.begin(); it != groups.end(); ++it)
	{
		for (Json& current : it.value())
		{
			addRawEntity(it.key())->loadFromConfig(current);
		}
	}

	if(!config["player"].is_null()) {
		setPlayerID(config["player"].get<int>());
	}
	else {
		_playerID = 0;
		_player = nullptr;
	}
	
	trySettingInitialUniqueID<Scene>(_id);
}

void Scene::saveToConfig(Json& config) const
{
	config["id"] = getID();
	config["name"] = getName();
	config["player"] = getPlayerID();

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
