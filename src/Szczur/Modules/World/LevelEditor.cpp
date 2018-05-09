//#ifdef EDITOR

#include "LevelEditor.hpp"

#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RectangleShape.hpp"

#include "Szczur/modules/FileSystem/FileDialog.hpp"

#include <ImGui/imgui.h>
#include <iostream>


namespace rat {
    LevelEditor::LevelEditor(SceneManager& scenes) :
	_scenes(scenes) {

    }

    void LevelEditor::render(sf3d::RenderTarget& target) {
		auto* scene = _scenes.getCurrentScene();
		if(scene) {
			_renderBar();
			if(_ifRenderObjectsList)
				_renderObjectsList();
			if(_anySelected)
				_renderFocusedObjectsParams();
			// if(_ifRenderComponentsManager)
				// _renderComponentsManager();
			if(_ifRenderDisplayDataManager)
				_renderDisplayDataManager();
			if(_ifRenderArmatureDisplayManager)
				_renderArmatureDisplayManager();

			sf3d::RectangleShape rect({100.f, 100.f});
			rect.setColor({1.f, 1.f, 0.f, 0.2f});
			rect.setOrigin({50.f, 50.f, 0.f});

			glDisable(GL_DEPTH_TEST);
			scene->forEach([this, &rect, &target](const std::string& group, Entity& entity){
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
		auto* scene = _scenes.getCurrentScene();
		bool cameraFocused = _focusedObject == scene->camera && _anySelected;
		if(ImGui::IsAnyItemActive() == false && !cameraFocused)
			_freeCamera.processEvents(input);
		sf3d::View view;
		if(cameraFocused) {
			auto* cam = scene->getEntity("single", scene->camera);
			view.setRotation(cam->getRotation());
			view.setCenter(cam->getPosition());
		}
		else {
			//std::cout << _freeCamera.position.x << ' ' << _freeCamera.position.y << '\n';
			view.setRotation(_freeCamera.rotation);
			view.setCenter(_freeCamera.position);
		}
		camera.setView(view);
    }

	void FreeCamera::processEvents(InputManager& input) {
		if(input.isKept(Keyboard::W)) {
			move({
				velocity * glm::sin(glm::radians(rotation.y)),
				0.f,
				-velocity * glm::cos(glm::radians(rotation.y))
			});
		}
		if(input.isKept(Keyboard::S))
			move({
				-velocity * glm::sin(glm::radians(rotation.y)),
				0.f,
				velocity * glm::cos(glm::radians(rotation.y))
			});
		if(input.isKept(Keyboard::D)) {
			move(glm::vec3{
				velocity * glm::cos(glm::radians(rotation.y)),
				0.f,
				velocity * glm::sin(glm::radians(rotation.y))
			});
		}
		if(input.isKept(Keyboard::A)) {
			move(glm::vec3{
				-velocity * glm::cos(glm::radians(rotation.y)),
				0.f,
				-velocity * glm::sin(glm::radians(rotation.y))
			});
		}
		if(input.isKept(Keyboard::Space))
			move({0.f, velocity, 0.f});
		if(input.isKept(Keyboard::LControl))
			move({0.f, -velocity, 0.f});
		if(rotating) {
			auto mouse = input.getMousePosition();
			rotate({
				(mouse.y - previousMouse.y)/10.f,
				(mouse.x - previousMouse.x)/10.f, 
				0.f
			});
			previousMouse = mouse;
		}
		if(input.isPressed(Mouse::Right)) {
			rotating = true;
			previousMouse = input.getMousePosition();
		}
		if(input.isReleased(Mouse::Right)) {
			rotating = false;
		}
	}

    void LevelEditor::_renderBar() {
		if(ImGui::BeginMainMenuBar()) {
			if(ImGui::BeginMenu("Files")) {
				static char buffer[255];
				if(ImGui::InputText("File", buffer, 255u)) {

				}
				if(ImGui::MenuItem("Load")) {
					try {
						_scenes.loadFromFile(buffer);
					}
					catch(...) {}
				}
				if(ImGui::MenuItem("Save")) {
					try {
						_scenes.saveToFile(buffer);
					}
					catch(...){}
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
				ImGui::MenuItem("Components Manager", nullptr, &_ifRenderComponentsManager);
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();
	}
    void LevelEditor::_renderDisplayDataManager() {
		static char enteredText[255];
		if(ImGui::Begin("Display Data Manager", &_ifRenderDisplayDataManager)) {
			auto& spriteDisplayDataHolder = _scenes.getCurrentScene()->getSpriteDisplayData();
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
						spriteDisplayDataHolder.erase(it);
						--it;
						continue;
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
			auto& armatureDisplayDataHolder = _scenes.getCurrentScene()->getArmatureDisplayData();
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
						armatureDisplayDataHolder.erase(it);
						--it;
						continue;
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
			auto* scene = _scenes.getCurrentScene();
			Entity* focusedObject = scene->getEntity(_focusedObject);

			// Change components
			if(ImGui::Button("Change components...")) {
				ImGui::OpenPopup("Change components...##modal");
				ImGui::SetNextWindowSize(ImVec2(300,300));
			}
			_renderComponentsManager();

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
							for(auto& it : scene->getSpriteDisplayData()) {
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
							for(auto& it : scene->getArmatureDisplayData()) {
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
				auto* scene = _scenes.getCurrentScene();
				auto& collectingHolder = scene->getAllEntities();
				for(auto& group : collectingHolder) {
					if(ImGui::SmallButton((std::string{"+###"} + std::to_string(i++)).c_str())) {
						Entity* ent = scene->addEntity(group.first);
						if(ent) {
							_focusedObject = ent->getID();
							_anySelected = true;
						}
						else
							_anySelected = false;
					}
					ImGui::SameLine();
					if(ImGui::TreeNode(group.first.c_str())) {
						//for(auto object = group.second.begin(); object != group.second.end(); ++object) {
						for(int i2 = 0; i2<group.second.size(); ++i2) {
							Entity& object = group.second[i2];
							bool temp = object.getID() == _focusedObject && _anySelected;
							if(ImGui::SmallButton(    (std::string{"-###"}+std::to_string(i++)).c_str()    )) {
								if(temp)
									_anySelected = false;
								group.second.erase(group.second.begin() + i2);
								--i2;
								continue;
							}
							ImGui::SameLine();
							if(ImGui::Selectable((object.getName()+"###"+std::to_string(i++)).c_str(), temp)) {
								if(temp) {
									_anySelected = false;
								}
								else {
									_focusedObject = object.getID();
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
	void LevelEditor::_renderComponentsManager() {
		if (ImGui::BeginPopupModal("Change components...##modal", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
			
			auto* focusedObject = _scenes.getCurrentScene()->getEntity(_focusedObject);

			static bool selectedComponents[2];
			if(ImGui::IsWindowAppearing()) {
				selectedComponents[0] = focusedObject->hasComponent<SpriteComponent>();
				selectedComponents[1] = focusedObject->hasComponent<ArmatureComponent>();
			}
			if(ImGui::Button("Accept", ImVec2(70,0))) {

				if(focusedObject->hasComponent<SpriteComponent>()!=selectedComponents[0]) {
					if(selectedComponents[0]) focusedObject->addComponent<SpriteComponent>();
					else focusedObject->removeComponent<SpriteComponent>();
				}
				if(focusedObject->hasComponent<ArmatureComponent>()!=selectedComponents[1]) {
					if(selectedComponents[1]) focusedObject->addComponent<ArmatureComponent>();
					else focusedObject->removeComponent<ArmatureComponent>();
				}

				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if(ImGui::Button("Close", ImVec2(70,0))) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::Checkbox("SpriteComponent", &selectedComponents[0]);
			ImGui::Checkbox("ArmatureComponent", &selectedComponents[1]);
			
			ImGui::EndPopup();
		}
	}
}

//#endif