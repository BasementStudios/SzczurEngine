//#ifdef EDITOR

#include "LevelEditor.hpp"

#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RectangleShape.hpp"

namespace rat {
    LevelEditor::LevelEditor() {

    }

	void LevelEditor::setScene(Scene* scene, size_t camera) {
		_scene = scene;
		if(scene) {
			_camera = camera;
		}
	}

    void LevelEditor::render(sf3d::RenderTarget& target) {
		if(_scene) {
			_renderBar();
			if(_ifRenderObjectsList)
				_renderObjectsList();
			if(_anySelected)
				_renderFocusedObjectsParams();
			if(_ifRenderModulesManager)
				_renderModulesManager();
			if(_ifRenderDisplayDataManager)
				_renderDisplayDataManager();
			if(_ifRenderArmatureDisplayManager)
				_renderArmatureDisplayManager();

			sf3d::RectangleShape rect({100.f, 100.f});
			rect.setColor({1.f, 1.f, 0.f, 0.2f});
			rect.setOrigin({50.f, 50.f, 0.f});

			glDisable(GL_DEPTH_TEST);
			_scene->forEach([this, &rect, &target](const std::string& group, Entity& entity){
				if(_focusedObject == entity.getID() && _anySelected)
					rect.setColor({1.f, 0.f, 1.f, 0.2f});
				else
					rect.setColor({1.f, 1.f, 0.f, 0.2f});
				rect.setPosition(entity.getPosition());
				target.draw(rect);
			}); 
			glEnable(GL_DEPTH_TEST);
		}
    }

    void LevelEditor::update(InputManager& input, Camera& camera) {
		if(input.isKept(Keyboard::W))
			_freeCamera.move({
				_freeCameraVelocity * glm::sin(glm::radians(_freeCamera.rotation.y)),
				0.f,
				-_freeCameraVelocity * glm::cos(glm::radians(_freeCamera.rotation.y))
			});
		if(input.isKept(Keyboard::S))
			_freeCamera.move({
				-_freeCameraVelocity * glm::sin(glm::radians(_freeCamera.rotation.y)),
				0.f,
				_freeCameraVelocity * glm::cos(glm::radians(_freeCamera.rotation.y))
			});
		if(input.isKept(Keyboard::D)) {
			_freeCamera.move(glm::vec3{
				_freeCameraVelocity * glm::cos(glm::radians(_freeCamera.rotation.y)),
				0.f,
				_freeCameraVelocity * glm::sin(glm::radians(_freeCamera.rotation.y))
			});
		}
		if(input.isKept(Keyboard::A)) {
			_freeCamera.move(glm::vec3{
				-_freeCameraVelocity * glm::cos(glm::radians(_freeCamera.rotation.y)),
				0.f,
				-_freeCameraVelocity * glm::sin(glm::radians(_freeCamera.rotation.y))
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
			auto* cam = _scene->getEntity("single", _camera);
			view.setRotation(cam->getRotation());
			view.setCenter(cam->getPosition());
		}
		else {
			view.setRotation(_freeCamera.rotation);
			view.setCenter(_freeCamera.position);
		}
		camera.setView(view);
    }



    void LevelEditor::_renderBar() {
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
				ImGui::MenuItem("Armature Data Manager", nullptr, &_ifRenderArmatureDisplayManager);
				ImGui::MenuItem("Modules Manager", nullptr, &_ifRenderModulesManager);
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();
	}
    void LevelEditor::_renderDisplayDataManager() {
		static char enteredText[255];
		if(ImGui::Begin("Display Data Manager", &_ifRenderDisplayDataManager)) {
			auto& spriteDisplayDataHolder = _scene->getSpriteDisplayData();
			if(ImGui::InputText("", enteredText, 255)) {
			}
			ImGui::SameLine();
			if(ImGui::Button("Add")) {
				try{
					spriteDisplayDataHolder.emplace_back(enteredText);
				}
				catch(const std::exception& exc) {
				}
				for(int i = 0; i<255; ++i)
					enteredText[i] = '\0';
			}
			ImGui::Separator();
			if(ImGui::BeginChild("Datas")) {
				for(auto it = spriteDisplayDataHolder.begin(); it!=spriteDisplayDataHolder.end(); ++it) {
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
    void LevelEditor::_renderArmatureDisplayManager() {
		static char enteredText[255];
		if(ImGui::Begin("Armature Data Manager", &_ifRenderArmatureDisplayManager)) {
			auto& armatureDisplayDataHolder = _scene->getArmatureDisplayData();
			if(ImGui::InputText("", enteredText, 255)) {
			}
			ImGui::SameLine();
			if(ImGui::Button("Add")) {
				try{
					armatureDisplayDataHolder.emplace_back(enteredText);
				}
				catch(const std::exception& exc) {
				}
				for(int i = 0; i<255; ++i)
					enteredText[i] = '\0';
			}
			ImGui::Separator();
			if(ImGui::BeginChild("Datas")) {
				for(auto it = armatureDisplayDataHolder.begin(); it!=armatureDisplayDataHolder.end(); ++it) {
					if(ImGui::SmallButton("-")) {
						//_armatureDisplayDataHolder.erase(it);
						//--it;
						//continue;
					}
					ImGui::SameLine();
					ImGui::Text(it->getName().c_str());
				}
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}
    void LevelEditor::_renderFocusedObjectsParams() {
		if(ImGui::Begin("Object Parameters", &_anySelected)) {
			Entity* focusedObject = _scene->getEntity(_focusedObject);
			if(focusedObject) {
				if(ImGui::CollapsingHeader("Base")) {
					glm::vec3 position = focusedObject->getPosition();
					glm::vec3 origin = focusedObject->getOrigin();
					origin.y = -origin.y;
					glm::vec3 rotation = focusedObject->getRotation();
					glm::vec3 scale = focusedObject->getScale();
					char name[255];
					std::strcpy(&name[0], focusedObject->getName().c_str());
					ImGui::InputText("", name, 255);
					focusedObject->setName(name);
					ImGui::DragFloat3("Position", reinterpret_cast<float*>(&position));
					ImGui::DragFloat3("Origin", reinterpret_cast<float*>(&origin));
					ImGui::DragFloat3("Rotation", reinterpret_cast<float*>(&rotation));
					ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&scale), 0.01f);
					//ImGui::Checkbox("Locked", &(_focusedObject->locked));
					focusedObject->setPosition(position);
					focusedObject->setOrigin(origin);
					focusedObject->setRotation(rotation);
					focusedObject->setScale(scale);
				}

				if(auto* object = focusedObject->getComponentAs<SpriteComponent>(); object != nullptr) {
					if(ImGui::CollapsingHeader("Sprite Component")) {
						if(ImGui::BeginCombo(
							"DisplayData",
							( object->getSpriteDisplayData() )?object->getSpriteDisplayData()->getName().c_str() : "None"
						)) {
							if(ImGui::MenuItem("None", nullptr, object->getSpriteDisplayData() == nullptr))
								object->setSpriteDisplayData(nullptr);
							for(auto& it : _scene->getSpriteDisplayData()) {
								if(ImGui::MenuItem(it.getName().c_str(), nullptr, object->getSpriteDisplayData() == &it))
									object->setSpriteDisplayData(&it);
							}

							ImGui::EndCombo();
						}
					}
				}

				if(auto* object = focusedObject->getComponentAs<ArmatureComponent>(); object != nullptr) {
					if(ImGui::CollapsingHeader("Armature Component")) {
						if(ImGui::BeginCombo(
							"DisplayData",
							( object->getArmatureDisplayData() )?object->getArmatureDisplayData()->getName().c_str() : "None"
						)) {
							if(ImGui::MenuItem("None", nullptr, object->getArmatureDisplayData() == nullptr))
								object->setArmatureDisplayData(nullptr);
							for(auto& it : _scene->getArmatureDisplayData()) {
								if(ImGui::MenuItem(it.getName().c_str(), nullptr, object->getArmatureDisplayData() == &it))
									object->setArmatureDisplayData(&it);
							}
							ImGui::EndCombo();
						}
						if(auto* arm = object->getArmature(); arm) {
							auto names = arm->getAnimation()->getAnimationNames();
							for(auto& it : names) {
								if(ImGui::Button(it.c_str())) {
									arm->getAnimation()->play(it);
								}
							}
						}
					}
				}
			}
		}
		ImGui::End();
	}
    void LevelEditor::_renderObjectsList() {
		if(ImGui::Begin("Objects", &_ifRenderObjectsList)){
			ImGui::Separator();
			if(ImGui::BeginChild("Objects")) {
				int i{0};
				auto& collectingHolder = _scene->getAllEntities();
				for(auto& group : collectingHolder) {
					if(ImGui::SmallButton((std::string{"+###"} + std::to_string(i++)).c_str())) {
						Entity* ent = _scene->addEntity(group.first);
						if(ent) {
							_focusedObject = ent->getID();
							_anySelected = true;
						}
						else
							_anySelected = false;
					}
					ImGui::SameLine();
					if(ImGui::TreeNode(group.first.c_str())) {
						for(auto object = group.second.begin(); object != group.second.end(); ++object) {
							bool temp = object->getID() == _focusedObject && _anySelected;
							if(ImGui::SmallButton("-")) {
								if(temp)
									_anySelected = false;
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
									_focusedObject = object->getID();
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
    void LevelEditor::_renderModulesManager() {
		if(ImGui::Begin("Modules Manager")) {
			if(!_anySelected) {
				ImGui::Text("Select Object First!");
			}
			else {
				auto* focusedObject = _scene->getEntity(_focusedObject);
				bool sC = focusedObject->hasComponent<SpriteComponent>();
				if(ImGui::Selectable("SpriteComponent", sC)) {
					if(sC)
						focusedObject->removeComponent<SpriteComponent>();
					else
						focusedObject->addComponent<SpriteComponent>();
				}
				sC = focusedObject->hasComponent<ArmatureComponent>();
				if(ImGui::Selectable("ArmatureComponent", sC)) {
					if(sC)
						focusedObject->removeComponent<ArmatureComponent>();
					else
						focusedObject->addComponent<ArmatureComponent>();
				}
			}
		}
		ImGui::End();
	}
}

//#endif