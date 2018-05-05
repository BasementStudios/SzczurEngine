#include "Scene.hpp"

#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Utility/SFML3D/Drawable.hpp"
#include "Szczur/Utility/SFML3D/Sprite.hpp"
#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/RectangleShape.hpp"

// #include <glm/glm.hpp>
// #include <ImGui/imgui.h>
// #include <SFML/Graphics.hpp>
// #include "SpriteComponent.hpp"

namespace rat
{

Scene::Scene()
	: _id { _getUniqueID() }
	, _name { "unnamed_" + std::to_string(_id) }
	, _collectingHolder { { "background", {} }, { "foreground", {} }, { "path", {} }, { "single", {} } }
{
	for (auto& holder : _collectingHolder)
	{
		holder.second.reserve(100);
	}
}

void Scene::update()
{
	for (auto& holder : _collectingHolder)
	{
		for (auto& entity : holder.second)
		{
			entity.update();
		}
	}
}

void Scene::render()
{
	for (auto& holder : _collectingHolder)
	{
		for (auto& entity : holder.second)
		{
			entity.render();
			// std::cout << entity.getID() << ' ' << entity.getName() << '\n';
		}
	}

	// #ifdef EDITOR
	// 	_renderBar();
	// 	if(_ifRenderObjectsList)
	// 		_renderObjectsList();
	// 	if(_anySelected)
	// 		_renderFocusedObjectsParams();
	// 	if(_ifRenderModulesManager)
	// 		_renderModulesManager();
	//
	// 	sf3d::RectangleShape rect({100.f, 100.f});
	// 	rect.setColor({1.f, 1.f, 0.f, 0.2f});
	// 	rect.setOrigin({50.f, 50.f, 0.f});
	//
	// 	auto& target = getModule<Window>().getWindow();
	// 	for (auto& group : _collectingHolder) {
	// 		for(auto object = group.second.begin(); object != group.second.end(); ++object) {
	// 			if(_focusedObject == object && _anySelected)
	// 				rect.setColor({1.f, 0.f, 1.f, 0.2f});
	// 			else
	// 				rect.setColor({1.f, 1.f, 0.f, 0.2f});
	// 			rect.setPosition(object->getPosition());
	// 			target.draw(rect);
	// 		}
	// 	}
	//
	// #endif
	// for (auto& holder : _collectingHolder) {
	// 	for (auto& entity : holder.second) {
	// 		if (auto ptr = entity.getFeature<sf3d::Drawable>(); ptr != nullptr) {
	// 			//std::cout << entity.getName() << '\n';
	// 		}
	// 	}
	// }
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

Entity* Scene::addEntity(const std::string group)
{
	return &_getSubHolder(group).emplace_back(this);
}

bool Scene::removeEntity(const std::string group, size_t id)
{
	if (auto it = _find(group, id); it != _getSubHolder(group).end())
	{
		_getSubHolder(group).erase(it);

		return true;
	}

	return false;
}

void Scene::removeAllEntities(const std::string group)
{
	_getSubHolder(group).clear();
}

void Scene::removeAllEntities()
{
	for (auto& holder : _collectingHolder)
	{
		holder.second.clear();
	}
}

Entity* Scene::getEntity(const std::string group, size_t id)
{
	if (auto it = _find(group, id); it != _getSubHolder(group).end())
	{
		return &(*it);
	}

	return nullptr;
}

const Entity* Scene::getEntity(const std::string group, size_t id) const
{
	if (auto it = _find(group, id); it != _getSubHolder(group).end())
	{
		return &(*it);
	}

	return nullptr;
}

bool Scene::hasEntity(const std::string group, size_t id)
{
	return _find(group, id) != _getSubHolder(group).end();
}

// void Scene::loadFromFile(const std::string& filepath)
// {
// 	std::ifstream file{ filepath };
// 	Json config;
//
// 	file >> config;
//
// 	for (auto it = config.begin(); it != config.end(); ++it)
// 	{
// 		for (Json& entity : it.value())
// 		{
// 			addEntity(it.key(), entity["name"]);
//
// 			getEntity(it.key(), entity["name"])->loadFromConfig(entity);
// 		}
// 	}
// }

// void Scene::saveToFile(const std::string& filepath) const
// {
// 	std::ofstream file{ filepath };
// 	Json config;
//
// 	for (auto& holder : _collectingHolder)
// 	{
// 		Json& currGroup = config[holder.first] = Json::array();
//
// 		for (auto& entity : holder.second)
// 		{
// 			currGroup.push_back(Json::object());
// 			Json& currEntity = currGroup.back();
//
// 			entity.saveToConfig(currEntity);
// 		}
// 	}
//
// 	file << std::setw(4) << config << std::endl;
// }

size_t Scene::_getUniqueID()
{
	static size_t id = 0;

	return id++;
}

Scene::EntitiesHolder_t& Scene::_getSubHolder(const std::string& group)
{
	return _collectingHolder.at(group);
}

const Scene::EntitiesHolder_t& Scene::_getSubHolder(const std::string& group) const
{
	return _collectingHolder.at(group);
}

typename Scene::EntitiesHolder_t::iterator Scene::_find(const std::string group, size_t id)
{
	auto& subHolder = _getSubHolder(group);

	return std::find_if(subHolder.begin(), subHolder.end(), [=](const auto& arg) {
		return arg.getID() == id;
	});
}

typename Scene::EntitiesHolder_t::const_iterator Scene::_find(const std::string group, size_t id) const
{
	const auto& subHolder = _getSubHolder(group);

	return std::find_if(subHolder.begin(), subHolder.end(), [=](const auto& arg) {
		return arg.getID() == id;
	});
}

// #ifdef EDITOR
//
// void Scene::_renderBar() {
// 	if(ImGui::BeginMainMenuBar()) {
// 		if(ImGui::BeginMenu("Files")) {
// 			if(ImGui::MenuItem("Load")) {
// 				std::cout << "LOAD\n";
// 			}
// 			if(ImGui::MenuItem("Save")) {
// 				std::cout << "Save\n";
// 			}
// 			if(ImGui::MenuItem("Save as")) {
// 				std::cout << "Save as\n";
// 			}
// 			if(ImGui::MenuItem("Exit")) {
// 				std::cout << "Exit\n";
// 			}
// 			ImGui::EndMenu();
// 		}
// 		if(ImGui::BeginMenu("Tools")) {
// 			ImGui::MenuItem("Objects List", nullptr, &_ifRenderObjectsList);
// 			ImGui::MenuItem("Modules Manager", nullptr, &_ifRenderModulesManager);
// 			ImGui::EndMenu();
// 		}
// 	}
// 	ImGui::EndMainMenuBar();
// }
//
// void Scene::_renderModulesManager() {
// 	if(ImGui::Begin("Modules Manager")) {
// 		if(!_anySelected) {
// 			ImGui::Text("Select Object First!");
// 		}
// 		else {
// 			bool sC = _focusedObject->hasComponent<SpriteComponent>();
// 			if(ImGui::Selectable("SpriteComponent", sC)) {
// 				if(sC)
// 					_focusedObject->removeComponent<SpriteComponent>();
// 				else
// 					_focusedObject->addComponent<SpriteComponent>();
// 			}
// 		}
// 	}
// 	ImGui::End();
// }
//
// void Scene::_renderFocusedObjectsParams() {
// 	if(ImGui::Begin("Object Parameters", &_anySelected)) {
// 		if(ImGui::CollapsingHeader("Base")) {
// 			glm::vec3 position = _focusedObject->getPosition();
// 			glm::vec3 origin = _focusedObject->getOrigin();
// 			origin.y = -origin.y;
// 			glm::vec3 rotation = _focusedObject->getRotation();
// 			glm::vec3 scale = _focusedObject->getScale();
// 			char name[255];
// 			std::strcpy(&name[0], _focusedObject->getName().c_str());
// 			ImGui::InputText("", name, 255);
// 			_focusedObject->setName(name);
// 			ImGui::DragFloat3("Position", reinterpret_cast<float*>(&position));
// 			ImGui::DragFloat3("Origin", reinterpret_cast<float*>(&origin));
// 			ImGui::DragFloat3("Rotation", reinterpret_cast<float*>(&rotation));
// 			ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&scale), 0.01f);
// 			_focusedObject->setPosition(position);
// 			_focusedObject->setOrigin(origin);
// 			_focusedObject->setRotation(rotation);
// 			_focusedObject->setScale(scale);
// 		}
//
// 		if(ImGui::CollapsingHeader("SpriteComponent")) {
// 			if(auto* it = _focusedObject->getComponentAs<SpriteComponent>(); it != nullptr) {
// 				//it->
// 			}
// 		}
//
// 	}
// 	ImGui::End();
// }
//
// void Scene::_renderObjectsList() {
// 	if(ImGui::Begin("Objects", &_ifRenderObjectsList)){
// 		ImGui::Separator();
// 		if(ImGui::BeginChild("Objects")) {
// 			int i{0};
// 			for(auto& group : _collectingHolder) {
// 				if(ImGui::SmallButton((std::string{"+###"} + std::to_string(i++)).c_str())) {
// 					auto it = addEntity(group.first);
// 					if(it != group.second.end()) {
// 						_focusedObject = it;
// 						_anySelected = true;
// 					}
// 					else
// 						_anySelected = false;
// 				}
// 				ImGui::SameLine();
// 				if(ImGui::TreeNode(group.first.c_str())) {
// 					for(auto object = group.second.begin(); object != group.second.end(); ++object) {
// 						bool temp = object == _focusedObject && _anySelected;
// 						if(ImGui::SmallButton("-")) {
// 							group.second.erase(object);
// 							--object;
// 							continue;
// 						}
// 						ImGui::SameLine();
// 						if(ImGui::Selectable((object->getName()+"###"+std::to_string(i++)).c_str(), temp)) {
// 							if(temp) {
// 								_anySelected = false;
// 							}
// 							else {
// 								_focusedObject = object;
// 								_anySelected = true;
// 							}
// 						}
// 					}
// 					ImGui::TreePop();
// 				}
// 				ImGui::Separator();
// 			}
// 		}
// 		ImGui::EndChild();
//
// 	}
// 	ImGui::End();
// }
// #endif

}
