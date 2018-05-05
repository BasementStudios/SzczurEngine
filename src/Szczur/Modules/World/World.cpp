#include "World.hpp"

/** @file World.cpp
 ** @description Implementation file with main class of the World module. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Utility/SFML3D/Drawable.hpp"

#include "Szczur/Utility/SFML3D/Sprite.hpp"
#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/RectangleShape.hpp"
#include <glm/glm.hpp>
#include <ImGui/imgui.h> 
#include <SFML/Graphics.hpp>
#include "SpriteComponent.hpp"

namespace rat
{

	/* Operators */
	/// Constructor
	World::World()
	: _collectingHolder{ { "background", {} }, { "foreground", {} }, { "path", {} }, { "single", {} } } {
		for (auto& holder : _collectingHolder)
		{
			holder.second.reserve(100);
		}
		_spriteDisplayDataHolder.reserve(100);

		_camera = addEntity("single", "Camera");
		_camera->setLock(true);
		_camera->setPosition({0.f, 0.f, 1000.f});
	}

	///
	void World::update()
	{
		auto& input = getModule<Input>().getManager();
		for (auto& holder : _collectingHolder)
		{
			for (auto& entity : holder.second)
			{
				//std::cout << entity.getName() << '\n';
			}
		}

		for (auto& holder : _collectingHolder) {
			for (auto& entity : holder.second) {
				if (auto ptr = entity.getComponentAs<InputControllerComponent>(); ptr != nullptr) {
					entity.move(ptr->update(input));
				}
			}
		}


		
		if(input.isKept(Keyboard::W))
			_freeCamera.move({
				_freeCameraVelocity * glm::sin(glm::radians(_freeCamera.getRotation().y)),
				0.f,
				-_freeCameraVelocity * glm::cos(glm::radians(_freeCamera.getRotation().y))
			});
		if(input.isKept(Keyboard::S))
			_freeCamera.move({
				-_freeCameraVelocity * glm::sin(glm::radians(_freeCamera.getRotation().y)),
				0.f,
				_freeCameraVelocity * glm::cos(glm::radians(_freeCamera.getRotation().y))
			});
		if(input.isKept(Keyboard::D)) {
			_freeCamera.move(glm::vec3{
				_freeCameraVelocity * glm::cos(glm::radians(_freeCamera.getRotation().y)),
				0.f,
				_freeCameraVelocity * glm::sin(glm::radians(_freeCamera.getRotation().y))
			});
		}
		if(input.isKept(Keyboard::A)) {
			_freeCamera.move(glm::vec3{
				-_freeCameraVelocity * glm::cos(glm::radians(_freeCamera.getRotation().y)),
				0.f,
				-_freeCameraVelocity * glm::sin(glm::radians(_freeCamera.getRotation().y))
			});
		}
		if(input.isKept(Keyboard::Space))
			_freeCamera.move({0.f, _freeCameraVelocity, 0.f});
		if(input.isKept(Keyboard::LControl))
			_freeCamera.move({0.f, -_freeCameraVelocity, 0.f});
		if(_rotatingCamera) {
			auto mouse = input.getMousePosition();
			_freeCamera.rotate({
				(mouse.y - _previousMouse.y)/10.f,
				(mouse.x - _previousMouse.x)/10.f, 
				0.f
			});
			_previousMouse = mouse;
		}
		if(input.isPressed(Mouse::Right)) {
			_rotatingCamera = true;
			_previousMouse = input.getMousePosition();
		}
		if(input.isReleased(Mouse::Right)) {
			_rotatingCamera = false;
		}


		sf3d::View view;
		if(_focusedObject == _camera && _anySelected) {
			view.setRotation(_camera->getRotation());
			view.setCenter(_camera->getPosition());
		}
		else {
			view.setRotation(_freeCamera.getRotation());
			view.setCenter(_freeCamera.getPosition());
		}
		getModule<Camera>().setView(view);
	}

	///
	void World::render()
	{
		auto& window = getModule<Window>().getWindow();
		for (auto& holder : _collectingHolder) {
			for (auto& entity : holder.second) {
				if (auto ptr = entity.getFeature<sf3d::Drawable>(); ptr != nullptr) {
					sf3d::RenderStates states = window.getDefaultRenderStates();
					states.transform *= entity.getTransform();
					window.draw(*ptr, states);
				}
			}
		}
		#ifdef EDITOR
			_renderBar();
			if(_ifRenderObjectsList)
				_renderObjectsList();
			if(_anySelected)
				_renderFocusedObjectsParams();
			if(_ifRenderModulesManager)
				_renderModulesManager();
			if(_ifRenderDisplayDataManager)
				_renderDisplayDataManager();

			sf3d::RectangleShape rect({100.f, 100.f});
			rect.setColor({1.f, 1.f, 0.f, 0.2f});
			rect.setOrigin({50.f, 50.f, 0.f});

			auto& target = getModule<Window>().getWindow();
			glDisable(GL_DEPTH_TEST);
			for (auto& group : _collectingHolder) {
				for(auto object = group.second.begin(); object != group.second.end(); ++object) {
					if(object->isLocked() == false) {
						if(_focusedObject == object && _anySelected)
							rect.setColor({1.f, 0.f, 1.f, 0.2f});
						else
							rect.setColor({1.f, 1.f, 0.f, 0.2f});
						rect.setPosition(object->getPosition());
						target.draw(rect);
					}
				}
			}
			glEnable(GL_DEPTH_TEST);
			
		#endif
	}

	World::EntitiesHolder_t::iterator World::addEntity(const std::string group, const std::string name) {
		auto& subGroup = _getSubHolder(group);
		if (auto it = _find(group, name); it == subGroup.end()) {
			subGroup.emplace_back(name);
			return subGroup.end()-1;
		}
		return subGroup.end();
	}

	Entity* World::getEntity(const std::string group, const std::string name) {
		if (auto it = _find(group, name); it != _getSubHolder(group).end())
		{
			return &(*it);
		}

		return nullptr;
	}

	const Entity* World::getEntity(const std::string group, const std::string name) const {
		if (auto it = _find(group, name); it != _getSubHolder(group).end())
		{
			return &(*it);
		}

		return nullptr;
	}

	///
	bool World::removeEntity(const std::string group, const std::string name) {
		if (auto it = _find(group, name); it != _getSubHolder(group).end())
		{
			_getSubHolder(group).erase(it);

			return true;
		}

		return false;
	}

	///
	void World::removeAllEntities(const std::string group){
		_getSubHolder(group).clear();
	}

	///
	void World::removeAllEntities()	{
		for (auto& holder : _collectingHolder) {
			holder.second.clear();
		}
	}

	///
	bool World::hasEntity(const std::string group, const std::string name) {
		return _find(group, name) != _getSubHolder(group).end();
	}


	///
	void World::loadFromFile(const std::string& filepath){
		std::ifstream file{ filepath };
		Json config;

		file >> config;

		for (auto it = config.begin(); it != config.end(); ++it)
		{
			for (Json& entity : it.value())
			{
				addEntity(it.key(), entity["name"]);

				getEntity(it.key(), entity["name"])->loadFromConfig(entity);
			}
		}
	}

	///
	void World::saveToFile(const std::string& filepath) const{
		std::ofstream file{ filepath };
		Json config;

		for (auto& holder : _collectingHolder) {
			Json& currGroup = config[holder.first] = Json::array();

			for (auto& entity : holder.second) {
				currGroup.push_back(Json::object());
				Json& currEntity = currGroup.back();

				entity.saveToConfig(currEntity);
			}
		}

		file << std::setw(4) << config << std::endl;
	}

	std::string World::_getUniqueName() {
		static size_t id = 0;

		return "unnamed_" + std::to_string(id++);
	}

	World::EntitiesHolder_t& World::_getSubHolder(const std::string& group) {
		return _collectingHolder.at(group);
	}

	const World::EntitiesHolder_t& World::_getSubHolder(const std::string& group) const	{
		return _collectingHolder.at(group);
	}

	typename World::EntitiesHolder_t::iterator World::_find(const std::string group, const std::string& name) {
		auto& subHolder = _getSubHolder(group);

		return std::find_if(subHolder.begin(), subHolder.end(), [&](const auto& arg) {
			return arg.getName() == name;
		});
	}

	typename World::EntitiesHolder_t::const_iterator World::_find(const std::string group, const std::string& name) const {
		const auto& subHolder = _getSubHolder(group);

		return std::find_if(subHolder.begin(), subHolder.end(), [&](const auto& arg) {
			return arg.getName() == name;
		});
	}

	#ifdef EDITOR

	void World::_renderBar() {
		if(ImGui::BeginMainMenuBar()) {
			if(ImGui::BeginMenu("Files")) {
				if(ImGui::MenuItem("Load")) {
					std::cout << "LOAD\n";
				}
				if(ImGui::MenuItem("Save")) {
					std::cout << "Save\n";
				}
				if(ImGui::MenuItem("Save as")) {
					std::cout << "Save as\n";
				}
				if(ImGui::MenuItem("Exit")) {
					std::cout << "Exit\n";
				}
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("Tools")) {
				ImGui::MenuItem("Objects List", nullptr, &_ifRenderObjectsList);
				ImGui::MenuItem("Display Data Manager", nullptr, &_ifRenderDisplayDataManager);
				ImGui::MenuItem("Modules Manager", nullptr, &_ifRenderModulesManager);
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();
	}

	void World::_renderModulesManager() {
		if(ImGui::Begin("Modules Manager")) {
			if(!_anySelected) {
				ImGui::Text("Select Object First!");
			}
			else {
				bool sC = _focusedObject->hasComponent<SpriteComponent>();
				if(ImGui::Selectable("SpriteComponent", sC)) {
					if(sC)
						_focusedObject->removeComponent<SpriteComponent>();
					else
						_focusedObject->addComponent<SpriteComponent>();
				}
				sC = _focusedObject->hasComponent<InputControllerComponent>();
				if(ImGui::Selectable("InputControllerComponent", sC)) {
					if(sC)
						_focusedObject->removeComponent<InputControllerComponent>();
					else
						_focusedObject->addComponent<InputControllerComponent>();
				}
			}
		}
		ImGui::End();
	}

	void World::_renderDisplayDataManager() {
		static char enteredText[255];
		if(ImGui::Begin("Display Data Manager", &_ifRenderDisplayDataManager)) {
			if(ImGui::InputText("", enteredText, 255)) {
			}
			ImGui::SameLine();
			if(ImGui::Button("Add")) {
				try{
					_spriteDisplayDataHolder.emplace_back(enteredText);
				}
				catch(const std::exception& exc) {
				}
				for(int i = 0; i<255; ++i)
					enteredText[i] = '\0';
			}
			ImGui::Separator();
			if(ImGui::BeginChild("Datas")) {
				for(auto it = _spriteDisplayDataHolder.begin(); it!=_spriteDisplayDataHolder.end(); ++it) {
					if(ImGui::SmallButton("-")) {
						//spriteDisplayData.erase(it);
						//--it;
					}
					ImGui::SameLine();
					ImGui::Text(it->getName().c_str());
				}
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}

	void World::_renderFocusedObjectsParams() {
		if(ImGui::Begin("Object Parameters", &_anySelected)) {
			if(ImGui::CollapsingHeader("Base")) {
				glm::vec3 position = _focusedObject->getPosition();
				glm::vec3 origin = _focusedObject->getOrigin();
				origin.y = -origin.y;
				glm::vec3 rotation = _focusedObject->getRotation();
				glm::vec3 scale = _focusedObject->getScale();
				char name[255];
				std::strcpy(&name[0], _focusedObject->getName().c_str());
				ImGui::InputText("", name, 255);
				_focusedObject->setName(name);
				ImGui::DragFloat3("Position", reinterpret_cast<float*>(&position));
				ImGui::DragFloat3("Origin", reinterpret_cast<float*>(&origin));
				ImGui::DragFloat3("Rotation", reinterpret_cast<float*>(&rotation));
				ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&scale), 0.01f);
				//ImGui::Checkbox("Locked", &(_focusedObject->locked));
				_focusedObject->setPosition(position);
				_focusedObject->setOrigin(origin);
				_focusedObject->setRotation(rotation);
				_focusedObject->setScale(scale);
			}

			if(auto* object = _focusedObject->getComponentAs<SpriteComponent>(); object != nullptr) {
				if(ImGui::CollapsingHeader("SpriteComponent")) {
					if(ImGui::BeginCombo(
						"DisplayData",
						( object->getSpriteDisplayData() )?object->getSpriteDisplayData()->getName().c_str() : "None"
					)) {
						if(ImGui::MenuItem("None", nullptr, object->getSpriteDisplayData() == nullptr))
							object->setSpriteDisplayData(nullptr);
						for(auto& it : _spriteDisplayDataHolder) {
							if(ImGui::MenuItem(it.getName().c_str(), nullptr, object->getSpriteDisplayData() == &it))
								object->setSpriteDisplayData(&it);
						}

						ImGui::EndCombo();
					}
				}
			}

			if(auto* object = _focusedObject->getComponentAs<InputControllerComponent>(); object != nullptr) {
				if(ImGui::CollapsingHeader("InputControllerComponent")) {
					
				}
			}

		}
		ImGui::End();
	}

	void World::_renderObjectsList() {
		if(ImGui::Begin("Objects", &_ifRenderObjectsList)){
			ImGui::Separator();
			if(ImGui::BeginChild("Objects")) {
				int i{0};
				for(auto& group : _collectingHolder) {
					if(ImGui::SmallButton((std::string{"+###"} + std::to_string(i++)).c_str())) {
						auto it = addEntity(group.first);
						if(it != group.second.end()) {
							_focusedObject = it;
							_anySelected = true;
						}
						else
							_anySelected = false;
						//_focusedObject->move({0.f, 0.f, -1000.f});
					}
					ImGui::SameLine();
					if(ImGui::TreeNode(group.first.c_str())) {
						for(auto object = group.second.begin(); object != group.second.end(); ++object) {
							bool temp = object == _focusedObject && _anySelected;
							if(ImGui::SmallButton("-")) {
								if(temp) {
									_anySelected = false;
								}
								group.second.erase(object);
								--object;
								continue;
							}
							ImGui::SameLine();
							if(ImGui::Selectable((object->getName()+"###"+std::to_string(i++)).c_str(), temp)) {
								if(temp) {
									_anySelected = false;
								}
								else {
									_focusedObject = object;
									_anySelected = true;
								}
							}
						}
						ImGui::TreePop();
					}
					ImGui::Separator();
				}
			}
			ImGui::EndChild();

		}
		ImGui::End();
	}
	#endif
	/* Methods*/
	/// init
	/*
	void World::init()
	{
		// @info For testing purposes
		{
			this->spriteDisplayData.reserve(20);
			for(auto& it : objects) {
				it.second.getContainer<Object<BaseObject, SpriteComponent>>().reserve(20);
				//it.second.getContainer<Object<BaseObject>>().reserve(20);
			}
			//_camera = &(this->objects["Singsle"].emplace(BaseObject{"Camera"}, SpriteDisplayData{nullptr}));
			_camera = &(objects["Single"].emplace(BaseObject{"Camera"}, SpriteComponent{nullptr}));
			_camera->locked = true;
			//this->objects["Path"].emplace(BaseObject{"locha"}, SpriteComponent{nullptr}).setPosition({0.f, 0.f, -1000.f});

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
		}
	}
	/// update
	void World::update(float deltaTime)
	{
		auto& input = getModule<Input>().getManager();

		if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			auto& target = getModule<Window>().getWindow();
			glm::vec2 mouse = {sf::Mouse::getPosition(target).x, sf::Mouse::getPosition(target).y};
			bool anyClicked{false};

			for(auto& it : objects) {
				it.second.forEach<BaseObject>([this, &anyClicked, &target, mouse](auto&& object) {
					if(!object.locked) {
						if(target.getLinerByScreenPos(mouse).contains(object.getPosition()-glm::vec3{50.f, -50.f, 0.f}, {100.f, 100.f, 0.f})) {
							_focusedObject = &object;
							anyClicked = true;
						}
						if(!anyClicked);
							//_focusedObject = nullptr;
					}
				});
			}
		}
		//this->pathObjects.forEach<InputController>([&input](auto&& object) {
			//object.update(input);
		//});
		#ifdef EDITOR
			_renderBar();
			if(_focusedObject)
				_renderFocusedObjectsParams();
			if(_ifRenderObjectsList)
				_renderObjectsList();
			if(_ifRenderDisplayDataManager)
				_renderDisplayDataManager();
		#endif
		sf3d::View view;
		view.setRotation(_camera->getRotation());
		view.setCenter(_camera->getPosition());
		//view.setCenter({1000.f, 0.f, 0.f});
		getModule<Camera>().setView(view);
	}

	/// render
	void World::render()
	{
		auto& target = getModule<Window>().getWindow(); // @todo . by render arg
		
		glEnable(GL_DEPTH_TEST);
		target.clear(37, 37, 48, 255, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		sf3d::RectangleShape rect({100.f, 100.f});
		rect.setColor({1.f, 1.f, 0.f, 0.2f});
		rect.setOrigin({50.f, 50.f, 0.f});

		for(auto& it : objects) {
			it.second.forEach<sf3d::Drawable>([&target](auto&& drawable) {
				target.draw(drawable);
			});
		}
		#ifdef EDITOR
			glDisable(GL_DEPTH_TEST);
			
			for(auto& it : objects) {
				it.second.forEach<BaseObject>([this, &target, &rect](auto&& object) {
					if(!object.locked) {
						if(_focusedObject == &object)
							rect.setColor({1.f, 0.f, 1.f, 0.2f});
						else
							rect.setColor({1.f, 1.f, 0.f, 0.2f});
						rect.setPosition(object.getPosition());
						target.draw(rect);
					}
				});
			}

			glEnable(GL_DEPTH_TEST);
		#endif
	}
	*/

}
