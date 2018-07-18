#include "ScenesManager.hpp"

#include <algorithm>
#include <experimental/filesystem>

#include <nlohmann/json.hpp>

#include "Components/CameraComponent.hpp"
#include "Components/BaseComponent.hpp"
#include "Components/ScriptableComponent.hpp"
#include "Components/PointLightComponent.hpp"

#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include "Szczur/Modules/World/World.hpp"
#include "Szczur/Modules/Cinematics/Cinematics.hpp"
#include "Szczur/Modules/Music/Music.hpp" 
#include "Szczur/Modules/Sound/Sound.hpp"
#include "Szczur/Modules/Equipment/Equipment.hpp"

#include "Szczur/Utility/SFML3D/LightPoint.hpp"

namespace rat
{

ScenesManager::ScenesManager() {
	_armatureDisplayDataHolder.reserve(100);
}

Scene* ScenesManager::addScene()
{
	Scene* scene = _holder.emplace_back(std::make_unique<Scene>(this)).get();

	// Add default player
	Entity* player = scene->addEntity("single");
	player->setName("Player");
	scene->setPlayer(player);
	
	// Add default camera
	Entity* camera = scene->addEntity("single");
	camera->addComponent<CameraComponent>();
	camera->setName("Camera");
	camera->setPosition({ 0.f, 1160.f, 3085.f });
	camera->setRotation({ 0.f, 0.f, 0.f });

	// Add default lighta
	Entity* sun = scene->addEntity("single");
	sun->setName("Sun");
	auto* comp = static_cast<PointLightComponent*>(sun->addComponent<PointLightComponent>());
	comp->setPosition({ 0.f, 0.f, 0.f });
	comp->setColor({1.f, 1.f, 1.f});
	comp->setAttenuation(sf3d::LightPoint::Attenuation{1.f, 0.f, 0.f});
	comp->setAmbientFactor({1.f, 1.f, 1.f});
	comp->setDiffuseFactor({1.f, 1.f, 1.f});
	comp->setSpecularFactor({1.f, 1.f, 1.f});

	return scene;
}

bool ScenesManager::removeScene(size_t id)
{
	if (auto it = _find(id); it != _holder.end())
	{
		_holder.erase(it);

		if (_currentSceneID == id)
		{
			if(_holder.size())
				_currentSceneID = (*_holder.begin())->getID();
			else
				_currentSceneID = addScene()->getID();
		}

		return true;
	}

	return false;
}

void ScenesManager::removeAllScenes()
{
	_holder.clear();

	_currentSceneID = 0u;

	#ifdef EDITOR
	detail::globalPtr<World>->getLevelEditor().getObjectsList().unselect();
	#endif //EDITOR
}

Scene* ScenesManager::getScene(size_t id) const
{
	if (auto it = _find(id); it != _holder.end())
	{
		return it->get();
	}

	return nullptr;
}

Scene* ScenesManager::getScene(const std::string& name) {

	for(auto& scene : _holder) {
		if(scene->getName() == name) {
			return scene.get();
		}
	}
	return nullptr;
}

ScenesManager::ScenesHolder_t& ScenesManager::getScenes()
{
	return _holder;
}

const ScenesManager::ScenesHolder_t& ScenesManager::getScenes() const
{
	return _holder;
}

bool ScenesManager::hasScene(size_t id) const
{
	return _find(id) != _holder.end();
}

bool ScenesManager::setCurrentScene(size_t id)
{
	if (hasScene(id))
	{
		#ifdef EDITOR
		detail::globalPtr<World>->getLevelEditor().getObjectsList().unselect();
		#endif //EDITOR
		
		_currentSceneID = id;

		if (isGameRunning()) {
			auto* scene = getCurrentScene();		
		
			scene->forEach([](const std::string& group, Entity& entity) {
					if (auto* comp = entity.getComponentAs<ScriptableComponent>()) comp->sceneChanged();
				}
			);
		}

		Window* window = detail::globalPtr<Window>;
		window->getWindow().setCamera(getCurrentScene()->getCamera()->getComponentAs<CameraComponent>());

		return true;
	}
	return false;
}

Scene* ScenesManager::getCurrentScene() const
{
	return getScene(_currentSceneID);
}

size_t ScenesManager::getCurrentSceneID() const
{
	return _currentSceneID;
}

bool ScenesManager::isCurrentSceneValid() const
{
	return _currentSceneID != 0u;
}

void ScenesManager::loadFromConfig(nlohmann::json& config) {

	removeAllScenes();

	_currentSceneID = config["currentSceneID"];

	nlohmann::json& scenes = config["scenes"];

	for (auto& current : scenes) {
		auto* scene = addScene();
		scene->removeAllEntities();
		scene->loadFromConfig(current);

		bool foundPlayer = false;
		bool foundCamera = false;
		
		for (auto& entity : scene->getEntities("single")) {
			if (entity->getName() == "Player") {
				foundPlayer = true;
				scene->setPlayer(entity.get());
			}
			else if (entity->getName() == "Camera") {
				foundCamera = true;
			}
		}

		if (!foundPlayer) {
			Entity* player = scene->addEntity("single");
			player->setName("Player");
			scene->setPlayer(player);
		}
		if (!foundCamera) {			
			Entity* camera = scene->addEntity("single");
			camera->addComponent<CameraComponent>();
			camera->setName("Camera");
			camera->setPosition({ 0.f, 1160.f, 3085.f });
			camera->setRotation({ 15.f, 0.f, 0.f });
		}
	}
}

void ScenesManager::saveToConfig(nlohmann::json& config) {

	config["version"] = std::string("1.6.7");
	config["currentSceneID"] = getCurrentSceneID();
	nlohmann::json& scenes = config["scenes"] = nlohmann::json::array();

	for (auto& scene : _holder)
	{
		scenes.push_back(nlohmann::json::object());
		nlohmann::json& current = scenes.back();

		scene->saveToConfig(current);
	}
}

void ScenesManager::saveEntityToConfig(Entity* entity, nlohmann::json& config) { 
	nlohmann::json& scenes = config["scenes"]; 
	int sceneID = entity->getScene()->getID(); 
	
	for(auto& scene : scenes) { 
		if(scene["id"].get<int>() == sceneID) { 
			auto& group = scene["groups"][entity->getGroup()]; 
			if(!group.is_null()) { 
				for(auto& ent : group) { 
					if(ent["id"].get<int>() == entity->getID()) 
					{ 
						ent.clear(); 
						entity->saveToConfig(ent); 
						return; 
					} 
				} 
			} 
	  		// Add with normal way 
			group.push_back(nlohmann::json::object()); 
			entity->saveToConfig(group.back()); 
			return; 
		} 
	} 
}

void ScenesManager::loadFromFile(const std::string& filepath)
{
	std::ifstream file{ filepath };
	nlohmann::json config;

	file >> config;

	loadFromConfig(config);
}

void ScenesManager::saveToFile(const std::string& filepath)
{
	std::ofstream file{ filepath };
	nlohmann::json config;

	saveToConfig(config);
	file << std::setw(4) << config << std::endl;
}

ScenesManager::ArmatureDisplayDataHolder_t& ScenesManager::getArmatureDisplayDataHolder()
{
	return _armatureDisplayDataHolder;
}

const ScenesManager::ArmatureDisplayDataHolder_t& ScenesManager::getArmatureDisplayDataHolder() const
{
	return _armatureDisplayDataHolder;
}

void ScenesManager::loadScenesFromFile(const std::string& filepath)
{
	std::ifstream file{ filepath };
	nlohmann::json config;

	file >> config;

	nlohmann::json& scenes = config["scenes"];

	for (auto& current : scenes)
	{
		auto* scene = addScene();
		scene->removeAllEntities();
		scene->loadFromConfig(current);
	}
}

void ScenesManager::addPlayer()
{
	auto scene = getCurrentScene();

	Entity* player = scene->addEntity("single");
	player->setName("Player");
	scene->setPlayer(player);
}

void ScenesManager::addCamera()
{
	auto scene = getCurrentScene();

	Entity* camera = scene->addEntity("single");
	camera->addComponent<CameraComponent>();
	camera->setName("Camera");
	camera->setPosition({ 0.f, 1160.f, 3085.f });
	camera->setRotation({ 0.f, 0.f, 0.f });
}

void ScenesManager::addSun()
{
	auto scene = getCurrentScene();

	Entity* sun = scene->addEntity("single");
	sun->setName("Sun");
	auto comp = static_cast<PointLightComponent*>(sun->addComponent<PointLightComponent>());
	comp->setPosition({ 0.f, 0.f, 0.f });
	comp->setColor({1.f, 1.f, 1.f});
	comp->setAttenuation(sf3d::LightPoint::Attenuation{1.f, 0.f, 0.f});
	comp->setAmbientFactor({1.f, 1.f, 1.f});
	comp->setDiffuseFactor({1.f, 1.f, 1.f});
	comp->setSpecularFactor({1.f, 1.f, 1.f});
}

bool ScenesManager::isGameRunning() {
	return _gameIsRunning;
}

void ScenesManager::runGame() {
	if(!_gameIsRunning) {
		_gameIsRunning = true;
		saveToConfig(_configBeforeRun);

		#ifdef EDITOR
		LevelEditor& levelEditor = detail::globalPtr<World>->getLevelEditor();
		#endif

		for (auto& scene : _holder) {
			scene->forEach([&](const std::string& group, Entity& entity) {
				if (auto comp = entity.getComponentAs<ScriptableComponent>()) comp->runScript();
				if (auto comp = entity.getComponentAs<AudioComponent>()) comp->play();

				#ifdef EDITOR
				if (levelEditor.reloadArmaturesAtStart()) {
					if (auto comp = entity.getComponentAs<ArmatureComponent>())
						comp->unloadArmature();
				}
				#endif
			});
		}

		#ifdef EDITOR
		if (levelEditor.reloadArmaturesAtStart()) {
			auto& arms = getArmatureDisplayDataHolder();

			for (auto& arm : arms) {
				arm->reload();
			}

			for (auto& scene : _holder) {
				scene->forEach([] (const std::string& group, Entity& entity) {
					if (auto comp = entity.getComponentAs<ArmatureComponent>()) comp->loadArmature();
				});
			}
		}
		#endif

		getCurrentScene()->forEach([] (const std::string& group, Entity& entity) {
			if (auto comp = entity.getComponentAs<ScriptableComponent>()) comp->sceneChanged();
		}
		);
	}
	detail::globalPtr<Equipment>->startEquipment();
}

void ScenesManager::stopGame() {
	if(_gameIsRunning) {
 
	    detail::globalPtr<Cinematics>->stop(); 
	    detail::globalPtr<Music>->stop(); 
	    detail::globalPtr<Sound>->stop(); 
		detail::globalPtr<Equipment>->stopEquipment();

		#ifdef EDITOR
		detail::globalPtr<World>->getLevelEditor().getObjectsList().unselect();
		#endif //EDITOR

		_gameIsRunning = false;
		loadFromConfig(_configBeforeRun);
	}
}

nlohmann::json& ScenesManager::getRunConfig() {
	return _configBeforeRun;
}

TextureDataHolder& ScenesManager::getTextureDataHolder() {
	return _textureDataHolder;
}

typename ScenesManager::ScenesHolder_t::iterator ScenesManager::_find(size_t id)
{
	return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
		return arg->getID() == id;
	});
}

typename ScenesManager::ScenesHolder_t::const_iterator ScenesManager::_find(size_t id) const
{
	return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
		return arg->getID() == id;
	});
}

#ifdef EDITOR
	bool ScenesManager::menuSave() {
		if(currentFilePath == "") {
			std::string relative = getRelativePathFromExplorer("Save world", ".\\Editor\\Saves", "Worlds (*.world)|*.world", true);
			// std::cout<<"--s-"<<relative<<std::endl;
			if(relative != "") {
				try {
					saveToFile(relative);
					currentFilePath = relative;
					return true;
				}
				catch (const std::exception& exc)
				{
					LOG_EXCEPTION(exc);
				}
			}
		}
		else {
			try {
				saveToFile(currentFilePath);
				return true;
			}
			catch (const std::exception& exc)
			{
				LOG_EXCEPTION(exc);
			}
		}

		return false;
	}

	std::string ScenesManager::getRelativePathFromExplorer(const std::string& title, const std::string& directory, const std::string& filter, bool saveButton) {
		namespace filesystem = std::experimental::filesystem;

		std::string file;
		if(saveButton) 
			file = FileDialog::getSaveFileName(title, directory, filter);
		else 
			file = FileDialog::getOpenFileName(title, directory, filter);

		if(file == "") 
			return "";

		std::string current = filesystem::current_path().string();

		if(current == file.substr(0, current.size()))
			return file.substr(current.size()+1);

		return "";
	}
#endif

}
