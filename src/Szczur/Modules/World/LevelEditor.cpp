#include "LevelEditor.hpp"

#include <iostream>
#include <experimental/filesystem>

#ifdef OS_WINDOWS
#include <Shellapi.h>
#endif

#include <ImGui/imgui.h>
#include <NodeEditor/NodeEditor.h>

#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RectangleShape.hpp"
#include "Szczur/Utility/SFML3D/CircleShape.hpp"

#include "Szczur/Utility/Convert/Windows1250.hpp"

#include "Szczur/Modules/Window/Window.hpp"

#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include "Szczur/Modules/FileSystem/DirectoryDialog.hpp"

#include "Szczur/Modules/Dialog/Dialog.hpp"

#include "Szczur/Modules/Trace/Timeline.hpp"
#include "Szczur/Modules/Trace/Actions/AnimAction.hpp"
#include "Szczur/Modules/Trace/Actions/MoveAction.hpp"

#include "Szczur/Modules/DialogEditor/DialogEditor.hpp"

namespace rat {
	LevelEditor::LevelEditor(SceneManager& scenes) :
	_scenes(scenes) {
		_freeCamera.move({1000.f,500.f,2000.f});
		detail::globalPtr<Window>->getWindow().setRenderDistance(300.f);
		_dialogEditor = detail::globalPtr<DialogEditor>;
		_audioEditor = detail::globalPtr<AudioEditor>;
	}

	void LevelEditor::render(sf3d::RenderTarget& target) {
		auto* scene = _scenes.getCurrentScene();
		if(scene) {
			_renderBar();
			if(_ifRenderObjectsList)
				_renderObjectsList();
			if(_anySelected)
				_renderFocusedObjectsParams();
			if(_ifRenderArmatureDisplayManager)
				_renderArmatureDisplayManager();
			if(_ifRenderDisplayDataManager)
				_renderDisplayDataManager();
			if (_ifRenderDialogEditor)
				_dialogEditor->update();
			if (_ifRenderAudioEditor)
				_audioEditor->render();

			scene = _scenes.getCurrentScene();
			sf3d::RectangleShape rect({100.f, 100.f});
			sf3d::CircleShape circ;
			sf3d::CircleShape circ2;
			circ.rotate({-90.f, 0.f, 0.f});
			circ.setColor({1.f, 0.f, 1.f, 0.2f});
			circ2.setColor({0.f, 1.f, 0.f, 0.2f});
			circ2.setRadius(50.f);
			circ2.setOrigin({50.f, 50.f, 0.f});
			rect.setColor({1.f, 1.f, 0.f, 0.2f});
			rect.setOrigin({50.f, 50.f, -10.f});

			//glDisable(GL_DEPTH_TEST);
			scene->forEach([&](const std::string& group, Entity& entity){
				rect.setPosition(entity.getPosition());
				if(_focusedObject == entity.getID() && _anySelected) {
					rect.setSize({100.f, 100.f});
					rect.setOrigin({50.f, 50.f, -10.f});
					rect.setColor({1.f, 0.3f, 1.f, 0.4f});
					target.draw(rect);
					rect.move({0.f, 0.f, 0.1f});
					rect.setSize({80.f, 80.f});
					rect.setOrigin({40.f, 40.f, -10.f});
					rect.setColor({0.f, 1.f, 1.f, 0.4f});
					target.draw(rect);
					rect.move({0.f, 0.f, -0.1f});
					// rect.setOutlineColor({1.f, 1.f, 0.f, 0.8f})
					// rect.setOutlineThickness(2.f);
				}
				else {
					rect.setSize({100.f, 100.f});
					rect.setOrigin({50.f, 50.f, -10.f});
					rect.setColor({0.7f, 0.f, 0.8f, 0.4f});
					target.draw(rect);
					rect.move({0.f, 0.f, 0.1f});
					rect.setSize({80.f, 80.f});
					rect.setOrigin({40.f, 40.f, -10.f});
					rect.setColor({1.f, 1.f, 0.f, 0.4f});
					target.draw(rect);
					rect.move({0.f, 0.f, -0.1f});
					// rect.setOutlineThickness(0.f);
				}
			});
			scene->forEach([&](const std::string& group, Entity& entity){
				if(auto* comp = entity.getComponentAs<InteractableComponent>(); comp != nullptr) {
					circ.setColor({1.f, 0.f, 1.f, 0.2f});
					circ.setPosition(entity.getPosition() + glm::vec3{0.f, comp->getHeight(), 0.f});
					float r = comp->getDistance();
					circ.setRadius(r);
					circ.setOrigin({r, r, 0.f});
					target.draw(circ);
				}
				if(auto* comp = entity.getComponentAs<TriggerComponent>(); comp != nullptr) {
					circ.setColor({1.f, 1.f, 0.f, 0.2f});
					circ.setPosition(entity.getPosition());
					float r = comp->getRadius();
					circ.setRadius(r);
					circ.setOrigin({r, r, 0.f});
					target.draw(circ);
				}
			});
			for(auto& it : _scenes.getCurrentScene()->getEntrances()) {
				circ2.setPosition(it.position);
				target.draw(circ2);
			}
			//glEnable(GL_DEPTH_TEST);
		}
	}

	void LevelEditor::update(InputManager& input, Camera& camera) {
		auto* scene = _scenes.getCurrentScene();
		sf3d::View view;
		Entity* currentCamera{nullptr};
		_scenes.getCurrentScene()->forEach([this, &currentCamera](const std::string&, Entity& entity){
			if(auto* component = entity.getComponentAs<CameraComponent>(); component != nullptr) {
				if((_focusedObject == entity.getID() && _anySelected) || component->getLock())
					currentCamera = &entity;
			}
		});
		if(ImGui::IsAnyItemActive() == false) {
			if(currentCamera==nullptr)
				_freeCamera.processEvents(input);
			else
				currentCamera->getComponentAs<CameraComponent>()->processEvents(input);
		}
		if(currentCamera) {
			view.setRotation(currentCamera->getRotation());
			view.setCenter(currentCamera->getPosition());
		}
		else {
			//std::cout << _freeCamera.position.x << ' ' << _freeCamera.position.y << '\n';
			view.setRotation(_freeCamera.rotation);
			view.setCenter(_freeCamera.position);
		}
		camera.setView(view);

		if(input.isReleased(Keyboard::F1)) {
			_menuSave();
		}
	}

	void FreeCamera::processEvents(InputManager& input) {

		if (detail::globalPtr<Dialog>->isDialogPlaying())
			return;

		velocity = 50.f;
		if(input.isKept(Keyboard::LShift)) {
			velocity = 200.f;
		}

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

		if (!ax::NodeEditor::IsActive())
		{
			if (input.isPressed(Mouse::Right))
			{
				rotating = true;
				previousMouse = input.getMousePosition();
			}
			if (input.isReleased(Mouse::Right))
			{
				rotating = false;
			}
		}
	}

	void LevelEditor::_renderBar() {
		// Create new world
		static bool openModalNew = false;
		static bool openScenesManager = false;
		static bool openSceneSettings = false;
		if(openModalNew) {
			openModalNew = false;
			ImGui::OpenPopup("Files/New##popup");
		}
		if(ImGui::BeginPopupModal("Files/New##popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("All those beautiful objects will be deleted.\nAre you sure you want to create a NEW world?\n\n");
			if(ImGui::Button("Yup!", ImVec2(120,0))) {
				_currentFilePath = "";
				_scenes.removeAllScenes();
				_scenes.setCurrentScene(_scenes.addScene()->getID());
				_anySelected = false;
				_focusedObject = static_cast<size_t>(-1);
				_printMenuInfo(std::string("New world created!"));
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if(ImGui::Button("Nope", ImVec2(120,0))) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		if(openScenesManager) {
			openScenesManager = false;
			ImGui::OpenPopup("Scenes Manager##popup");
		}
		if(ImGui::BeginPopupModal("Scenes Manager##popup")) {
			static char name[255];

			std::strcpy(&name[0], _scenes.getCurrentScene()->getName().c_str());
			ImGui::InputText("", name, 255u);
			_scenes.getCurrentScene()->setName(std::string{name});
			if(ImGui::Button("Add")) {
				_scenes.addScene();
			}
			ImGui::SameLine();
			if(ImGui::Button("Remove")) {
				_scenes.removeScene(_scenes.getCurrentSceneID());
				_anySelected = false;
			}
			ImGui::SameLine();
			if(ImGui::Button("Ok")) {
				ImGui::CloseCurrentPopup();
			}
			if(ImGui::BeginChild("Scenes")) {
				for(auto& it : _scenes.getScenes()) {
					if(ImGui::Selectable(it->getName().c_str(), it->getID() == _scenes.getCurrentSceneID())) {
						_scenes.setCurrentScene(it->getID());
						_anySelected = false;
					}
				}
			}
			ImGui::EndChild();
			ImGui::EndPopup();
		}

		if(openSceneSettings) {
			openSceneSettings = false;
			ImGui::OpenPopup("Scene Settings##popup");
		}
		if(ImGui::BeginPopupModal("Scene Settings##popup")) {
			auto& holder = _scenes.getCurrentScene()->getEntrances();
			if(ImGui::Button("Add")) {

				//holder.push_back(std::make_pair(std::string{""}, glm::vec3{0.f, 0.f, 0.f}));
				holder.push_back(Scene::Entrance{getUniqueID<Scene::Entrance>(), "", glm::vec3(0.f, 0.f, 0.f)});
			}
			ImGui::SameLine();
			if(ImGui::Button("Ok")) {
				ImGui::CloseCurrentPopup();
			}

			if(ImGui::BeginChild("List##list")) {
				int i = 0;
				holder.erase(
					std::remove_if(holder.begin(), holder.end(), [&i](Scene::Entrance& it){
						if(ImGui::Button( (std::string{"-##"}+std::to_string(i++)).c_str() ))
							return true;
						ImGui::SameLine();
						char name[255];
						std::strcpy(&name[0], it.name.c_str());
						ImGui::InputText((std::string{"##"}+std::to_string(i++)).c_str(), name, 255);
						it.name = std::string{name};
						ImGui::DragFloat3((std::string{"##"}+std::to_string(i++)).c_str(), (float*)(&(it.position)));
						ImGui::Separator();
						return false;

					}), holder.end()
				);

			}
			ImGui::EndChild();

			ImGui::EndPopup();
		}
		if(ImGui::BeginMainMenuBar()) {
			if(ImGui::BeginMenu("Files")) {
				if(ImGui::MenuItem("New")) {
					openModalNew = true;
				}
				if(ImGui::MenuItem("Load")) {
					std::string relative = _getRelativePathFromExplorer("Load world", ".\\Editor\\Saves", "Worlds (*.world)|*.world");
					// std::cout<<"--l-"<<relative<<std::endl;
					if(relative != "") {
						try {
							_scenes.loadFromFile(relative);
							_focusedObject = -1;
							_anySelected = false;
							_currentFilePath = relative;
							_printMenuInfo(std::string("World loaded from file: ")+_currentFilePath);
						}
						catch (const std::exception& exc)
						{
							LOG_EXCEPTION(exc);
						}
					}
				}
				if(ImGui::MenuItem("Save", "F1")) {
					_menuSave();
				}
				if(ImGui::MenuItem("Save As")) {
					std::string relative = _getRelativePathFromExplorer("Save world", ".\\Editor\\Saves", "Worlds (*.world)|*.world", true);
					// std::cout<<"--s-"<<relative<<std::endl;
					if(relative != "") {
						try {
							_scenes.saveToFile(relative);
							_currentFilePath = relative;
							_printMenuInfo(std::string("World saved in file: ")+_currentFilePath);
						}
						catch (const std::exception& exc)
						{
							LOG_EXCEPTION(exc);
						}
					}
				}

				if (ImGui::MenuItem("Show in explorer")) {
					std::string current = std::experimental::filesystem::current_path().string();

#ifdef OS_WINDOWS
					ShellExecute(NULL, "open", current.c_str(), NULL, NULL, SW_SHOWDEFAULT);
#endif
				}
				if(ImGui::MenuItem("Scenes Manager", "")) {
					openScenesManager = true;
				}
				if(ImGui::MenuItem("Scenes Settings", "")) {
					openSceneSettings = true;
				}

				ImGui::Separator();

				if(ImGui::MenuItem("Exit")) {
					std::cout << "Exit\n";
				}
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("Tools")) {
				ImGui::MenuItem("Objects List", nullptr, &_ifRenderObjectsList);
				ImGui::MenuItem("Display Data Manager", nullptr, &_ifRenderDisplayDataManager);
				ImGui::MenuItem("Armature Data Manager", nullptr, &_ifRenderArmatureDisplayManager);
				ImGui::MenuItem("Dialog Editor", nullptr, &_ifRenderDialogEditor);
				ImGui::MenuItem("Audio Editor", nullptr, &_ifRenderAudioEditor);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Debug"))
			{
				if (ImGui::MenuItem("Add player", nullptr))
				{
					_scenes.addPlayer();
				}

				ImGui::EndMenu();
			}

			// Menu info
			if(_menuInfoClock.getElapsedTime().asSeconds()<6.0f) {
				ImGui::SameLine(0, 16);
				ImGui::TextColored({0.75f, 0.30f, 0.63f, 1.0f - _menuInfoClock.getElapsedTime().asSeconds()/6.0f}, _menuInfo.c_str());
			}

			ImGui::SameLine(ImGui::GetWindowWidth()-72);
			ImGui::Text((std::to_string((int)ImGui::GetIO().Framerate)+" FPS").c_str());
		}
		ImGui::EndMainMenuBar();
	}

	void LevelEditor::_printMenuInfo(const std::string& info) {
		_menuInfo = info;
		_menuInfoClock.restart();
	}

	void LevelEditor::_renderDisplayDataManager() {
		static char enteredText[255];
		if(ImGui::Begin("Display Data Manager", &_ifRenderDisplayDataManager)) {
			auto& spriteDisplayDataHolder = _scenes.getCurrentScene()->getSpriteDisplayDataHolder();
			if(ImGui::InputText("", enteredText, 255)) {
			}
			ImGui::SameLine();
			if(ImGui::Button("Add")) {
				try{
					spriteDisplayDataHolder.emplace_back(enteredText);
				}
				catch (const std::exception& exc)
				{
					LOG_EXCEPTION(exc);
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
			auto& armatureDisplayDataHolder = _scenes.getArmatureDisplayDataHolder();
			if(ImGui::InputText("", enteredText, 255)) {
			}
			ImGui::SameLine();
			if(ImGui::Button("Add")) {
				try{
					armatureDisplayDataHolder.emplace_back(enteredText);
				}
				catch (const std::exception& exc)
				{
					LOG_EXCEPTION(exc);
				}
				for(int i = 0; i<255; ++i)
					enteredText[i] = '\0';
			}
			ImGui::Separator();
			if(ImGui::BeginChild("Datas")) {
				for(auto it = armatureDisplayDataHolder.begin(); it!=armatureDisplayDataHolder.end(); ++it) {
					if(ImGui::SmallButton("-")) {
						armatureDisplayDataHolder.erase(it);
						//--it;

						break;
					}
					ImGui::SameLine();
					ImGui::Text(mapWindows1250ToUtf8(it->getName()).c_str());
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
				if(ImGui::CollapsingHeader("Base", ImGuiTreeNodeFlags_DefaultOpen)) {
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
					static bool lockRatio{false};
					ImGui::DragFloat2("", reinterpret_cast<float*>(&scale), 0.01f);
					ImGui::SameLine(0.f, 0.f);
					ImGui::Checkbox("Scale##CheckboxLockRatio", &lockRatio);
					//ImGui::Checkbox("Locked", &(_focusedObject->locked));
					focusedObject->setPosition(position);
					focusedObject->setOrigin(origin);
					focusedObject->setRotation(rotation);
					if(lockRatio == false) {
						focusedObject->setScale(scale);
					}
					else {
						float offset = (scale.x / focusedObject->getScale().x) + (scale.y / focusedObject->getScale().y) - 1;
						focusedObject->scale( {offset, offset, 1.f} );
					}
				}

				if(auto* object = focusedObject->getComponentAs<SpriteComponent>(); object != nullptr) {
					if(ImGui::CollapsingHeader("Sprite Component")) {

						// Load texture button
						auto& spriteDisplayDataHolder = _scenes.getCurrentScene()->getSpriteDisplayDataHolder();
						if(ImGui::Button("Load texture...##sprite_component")) {
							std::string file = _getRelativePathFromExplorer("Select texture", ".\\Assets");
							std::cout<<file<<std::endl;
							if(file != "") {
								try {
									auto& it = spriteDisplayDataHolder.emplace_back(file);
									object->setSpriteDisplayData(&it);
								}
								catch(const std::exception& exc) {
									object->setSpriteDisplayData(nullptr);

									LOG_EXCEPTION(exc);
								}
							}
						}
						ImGui::Text("Path:");
						ImGui::SameLine();
						ImGui::Text(object->getSpriteDisplayData() ? mapWindows1250ToUtf8(object->getSpriteDisplayData()->getName()).c_str() : "None");
					}
				}

				if(auto* object = focusedObject->getComponentAs<CameraComponent>(); object != nullptr) {
					if(ImGui::CollapsingHeader("Camera Component")) {
						float velocity = object->getVelocity();
						bool locked = object->getLock();
						bool stickToPlayer = object->getStickToPlayer();
						ImGui::DragFloat("Velocity", &velocity);
						ImGui::Checkbox("Locked", &locked);
						ImGui::Checkbox("Stick To Player", &stickToPlayer);

						object->setVelocity(velocity);
						object->setLock(locked);
						object->setStickToPlayer(stickToPlayer);

					}
				}


				if(auto* object = focusedObject->getComponentAs<ArmatureComponent>(); object != nullptr) {
					if(ImGui::CollapsingHeader("Armature Component")) {

						//Load armature button
						auto& armatureDisplayDataHolder = _scenes.getArmatureDisplayDataHolder();
						if(ImGui::Button("Load armature...##armature_component")) {
							// std::string directory = _getRelativeDirectoryPathFromExplorer("Select armature folder", "Assets");
							std::string directory = _getRelativePathFromExplorer("Select armature file", ".\\Assets");
							directory = std::experimental::filesystem::path(directory).parent_path().string();
							if(directory != "") {
								try {

									auto item = std::find_if(armatureDisplayDataHolder.begin(), armatureDisplayDataHolder.end(), [directory] (auto& item) { return item.getFolderPath() == directory; });

									ArmatureDisplayData* it = nullptr;

									if (item == armatureDisplayDataHolder.end())
									{
										// create new
										it = &armatureDisplayDataHolder.emplace_back(directory);
									}
									else
									{
										// use exisiting
										it = &(*item);
									}

									object->setArmatureDisplayData(it);
								}
								catch(const std::exception& exc) {
									object->setArmatureDisplayData(nullptr, false);

									LOG_EXCEPTION(exc);
								}
							}
						}
						ImGui::Text("Path:");
						ImGui::SameLine();
						ImGui::Text(object->getArmatureDisplayData() ? mapWindows1250ToUtf8(object->getArmatureDisplayData()->getName()).c_str() : "None");

						// Select animation button
						if(auto* arm = object->getArmature(); arm && arm->getAnimation()) {
							ImGui::Button("Select animation##armature_component");

							ImGui::SetNextWindowSize({300.f, 200.f});
							if(ImGui::BeginPopupContextItem(NULL, 0)) {
								auto names = arm->getAnimation()->getAnimationNames();
								for(auto& it : names) {
									ImGui::PushID(it.c_str());
									if(ImGui::Selectable(it.c_str())) {
										arm->getAnimation()->play(it);
									}
									ImGui::PopID();
								}
								ImGui::EndPopup();
							}
							if(arm->getAnimation()->isPlaying()) {
								ImGui::SameLine();
								if(ImGui::Button("Stop##armature_component")) {
									arm->getAnimation()->play(arm->getAnimation()->getLastAnimationName());
									arm->getAnimation()->stop(arm->getAnimation()->getLastAnimationName());
								}
							}

							ImGui::DragFloat("Animation speed##armature_component", &arm->getAnimation()->timeScale, 0.01f);
						}


					}
				}
				if(auto* object = focusedObject->getComponentAs<ScriptableComponent>(); object != nullptr) {
					if(ImGui::CollapsingHeader("Scriptable Component")) {
						static bool onceType = false;
						if(ImGui::Button("Load")) {
							std::string file = _getRelativePathFromExplorer("Select script file", ".\\Assets");

							if(file != "") {
								try {
									if(onceType) {
										object->loadAnyScript(file);
									}
									else {
										object->loadScript(file);
									}
								}
								catch(const std::exception& exc) {
									LOG_EXCEPTION(exc);
								}
							}
						}
						if(object->getFilePath()!="") {
							ImGui::SameLine();
							if(ImGui::Button("Reload##scriptable_component")) {
								try {
									object->reloadScript();
								}
								catch (const std::exception& exc)
								{
									LOG_EXCEPTION(exc);
								}
							}
							ImGui::SameLine();
							if(ImGui::Button("Remove##scriptable_component")) {
								object->loadScript("");
							}
						}
						ImGui::Text("Path:");
						ImGui::SameLine();
						ImGui::Text(object->getFilePath()!="" ? mapWindows1250ToUtf8(object->getFilePath()).c_str() : "None");

						ImGui::Checkbox("Once type##scriptable_component", &onceType);
					}
				}

				if(auto* object = focusedObject->getComponentAs<InteractableComponent>(); object != nullptr) {
					if(ImGui::CollapsingHeader("Interactable Component")) {
						float distance = object->getDistance();
						float height = object->getHeight();
						ImGui::DragFloat("Distance", &distance);
						ImGui::DragFloat("Height", &height);
						object->setDistance(distance);
						object->setHeight(height);

					}
				}
				if(auto* object = focusedObject->getComponentAs<TriggerComponent>(); object != nullptr) {
					if(ImGui::CollapsingHeader("Trigger Component")) {
						float radius = object->getRadius();
						if(ImGui::BeginCombo("Types", TriggerComponent::enumToString(object->type).c_str())) {
							if(ImGui::Selectable(TriggerComponent::enumToString(TriggerComponent::None).c_str(), object->type == TriggerComponent::None)) {
								object->type = TriggerComponent::None;
							}
							if(
								ImGui::Selectable(TriggerComponent::enumToString(
									TriggerComponent::ChangeScene).c_str(),
									object->type == TriggerComponent::ChangeScene)
								) {
								object->type = TriggerComponent::ChangeScene;
								object->sceneId = 0u;
								object->entranceId = 0u;
							}
							ImGui::EndCombo();
						}
						ImGui::DragFloat("Radius", &radius);
						object->setRadius(radius);
						if (object->type == TriggerComponent::ChangeScene)
						{
							auto* temp = _scenes.getScene(object->sceneId);
							if (ImGui::BeginCombo("Scene", temp ? temp->getName().c_str() : "None"))
							{
								for (auto& it : _scenes.getScenes())
								{
									if (ImGui::Selectable(it->getName().c_str(), object->sceneId == it->getID()))
									{
										object->sceneId = it->getID();
									}
								}

								ImGui::EndCombo();
							}
							if (object->sceneId)
							{
								auto& holder = _scenes.getScene(object->sceneId)->getEntrances();
								std::string name = "None";
								for (auto& it : holder)
								{
									if (it.ID == object->entranceId)
									{
										name = it.name;
										break;

									}
									if (ImGui::BeginCombo("Entrance", name.c_str()))
									{
										for (auto& it : holder)
										{
											if (ImGui::Selectable(it.name.c_str(), it.ID == object->entranceId))
											{
												object->entranceId = it.ID;
												ImGui::EndCombo();
											}
										}
									}
								}
							}
						}
					}
				}

				if (auto* object = focusedObject->getComponentAs<TraceComponent>(); object != nullptr)
				{
					if (ImGui::CollapsingHeader("Trace Component"))
					{
						if (ImGui::TreeNode("Traces"))
						{
							for (auto& timeline : object->getTrace()->getTimelines())
							{
								std::string id = std::to_string(reinterpret_cast<uintptr_t>(timeline.get()));

								if (ImGui::TreeNode(("Timeline##" + id).c_str()))
								{
									ImGui::SameLine();

									if (ImGui::SmallButton(("Use##" + id).c_str()))
									{
										object->getTrace()->setCurrentTimeline(timeline.get());
									}

									ImGui::Checkbox("Loop", &timeline->Loop);
									ImGui::Checkbox("Show lines in editor", &timeline->ShowLines);

									ImGui::Separator();

									for (auto& action : timeline->getActions())
									{
										std::string actionId = std::to_string(reinterpret_cast<uintptr_t>(action.get()));

										switch (action->getType())
										{
											case Action::Move:
											{
												auto moveAction = static_cast<MoveAction*>(action.get());

												if (ImGui::TreeNode(("Move##" + actionId).c_str()))
												{
													ImGui::Checkbox("Use current position as start position", &moveAction->UseCurrentPosition);

													if (!moveAction->UseCurrentPosition)
														ImGui::DragFloat3(("Start##" + actionId).c_str(), reinterpret_cast<float*>(&moveAction->Start));

													ImGui::DragFloat3(("End##" + actionId).c_str(), reinterpret_cast<float*>(&moveAction->End));
													ImGui::DragFloat(("Speed##" + actionId).c_str(), &moveAction->Speed, 0.25f);
													ImGui::Checkbox(("Teleport##" + actionId).c_str(), &moveAction->Teleport);

													ImGui::TreePop();
												}
											} break;
											case Action::Anim:
											{
												auto animAction = static_cast<AnimAction*>(action.get());

												if (ImGui::TreeNode(("Anim##" + actionId).c_str()))
												{
													static char animName[64] = { 0 };
													strcpy(animName, animAction->AnimationName.c_str());

													if (ImGui::InputText(("Animation name##" + actionId).c_str(), animName, 64))
													{
														animAction->AnimationName = animName;
													}
													ImGui::DragFloat(("Fade in time##" + actionId).c_str(), &animAction->FadeInTime, 0.01f);

													ImGui::Checkbox(("Play once##" + actionId).c_str(), &animAction->PlayOnce);

													if (animAction->PlayOnce)
														ImGui::Checkbox(("Wait to the end of animation##" + actionId).c_str(), &animAction->WaitToEnd);

													ImGui::Checkbox(("Flip X##" + actionId).c_str(), &animAction->FlipX);

													ImGui::TreePop();
												}
											} break;
										
										}

									}

									static int currentAction = -1;

									ImGui::PushItemWidth(100.f);
									ImGui::Combo(("##Action" + id).c_str(), &currentAction, "Move\0Anim\0Wait\0");

									ImGui::SameLine();

									if (ImGui::Button(("Add action##" + id).c_str()))
									{
										if (currentAction == 0)
										{
											timeline->addAction(new MoveAction(focusedObject));
										}
										else if (currentAction == 1)
										{
											if (focusedObject->hasComponent<ArmatureComponent>())
												timeline->addAction(new AnimAction(focusedObject));
										}
										
									}

									ImGui::TreePop();
								}
							}
							ImGui::TreePop();
						}

						ImGui::Separator();

						if (ImGui::Button("Add timeline"))
						{
							object->getTrace()->addTimeline(new Timeline());
						}

						ImGui::SameLine();

						if (ImGui::Button("Stop##trace_component"))
						{
							object->getTrace()->setCurrentTimeline(nullptr);
						}
					}


				}
			}
		}
		ImGui::End();
	}
	void LevelEditor::_renderObjectsList() {
		if(ImGui::Begin("Objects", &_ifRenderObjectsList)) {
			ImGui::Separator();
			if(ImGui::BeginChild("Objects")) {
				int i = 0;
				auto* scene = _scenes.getCurrentScene();
				for(auto& group : scene->getAllEntities()) {
					ImGui::PushID(i);

					ImGui::PushStyleColor(ImGuiCol_Header, {0.f, 0.f, 0.f, 0.f});
					if(ImGui::TreeNodeEx(group.first.c_str(), ImGuiTreeNodeFlags_FramePadding|ImGuiTreeNodeFlags_DefaultOpen|ImGuiTreeNodeFlags_Framed)) {
						ImGui::PopStyleColor();

						// Context menu for group. Options: Add object
						if(group.first != "single" && ImGui::BeginPopupContextItem("Group context menu")) {
							if(ImGui::Selectable("Add object##group_context_menu")) {
								Entity* ent = scene->addEntity(group.first);
								if(ent) {
									_focusedObject = ent->getID();
								}
								_anySelected = static_cast<bool>(ent);
							}
							ImGui::EndPopup();
						}


						for(int i2 = 0; i2<group.second.size(); ++i2) {
							Entity& object = *group.second[i2];
							bool temp = object.getID() == _focusedObject && _anySelected;
							ImGui::PushID(i2);
							if(ImGui::Selectable(object.getName().c_str(), temp)) {
								if(!temp) {
									_focusedObject = object.getID();
								}
								_anySelected = !temp;
							}

							// Swapping objects in list
							static int draggedObject = -1;
							static std::string draggedCategory;
							if(ImGui::IsMouseReleased(0)) {
								draggedObject = -1;
							}
							if(ImGui::IsItemClicked()) {
								draggedObject = i2;
								draggedCategory = group.first;
							}
							if(ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)) {
								if(draggedObject != -1 && draggedObject != i2 && draggedCategory == group.first) {
									std::swap(group.second[i2], group.second[draggedObject]);
									draggedObject = i2;
								}
							}

							// Context menu for object. Options: Duplicate|Remove
							if(group.first != "single") {
								if(ImGui::BeginPopupContextItem("Object context menu")) {
									if(ImGui::Selectable("Duplicate##object_context_menu")) {
										Entity* ent = scene->duplicateEntity(group.second[i2]->getID());
										if(ent) {
											_focusedObject = ent->getID();
										}
										_anySelected = static_cast<bool>(ent);
									}
									if(ImGui::Selectable("Remove##object_context_menu")) {
										if(temp)
											_anySelected = false;
										group.second.erase(group.second.begin() + i2);
										ImGui::EndPopup();
										ImGui::PopID();
										break;
									}
									ImGui::EndPopup();
								}
							}

							ImGui::PopID();
						}
						ImGui::TreePop();
					}
					else {
						ImGui::PopStyleColor();

						// Context menu for group. Options: Add object
						if(group.first != "single" && ImGui::BeginPopupContextItem("Group context menu")) {
							if(ImGui::Selectable("Add object##group_context_menu")) {
								Entity* ent = scene->addEntity(group.first);
								if(ent) {
									_focusedObject = ent->getID();
								}
								_anySelected = static_cast<bool>(ent);
							}
							ImGui::EndPopup();
						}

					}
					ImGui::Separator();
					ImGui::PopID();
					++i;
				}
			}
			ImGui::EndChild();

		}
		ImGui::End();
	}
	void LevelEditor::_renderComponentsManager() {
		if (ImGui::BeginPopupModal("Change components...##modal", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {

			auto* focusedObject = _scenes.getCurrentScene()->getEntity(_focusedObject);

			static bool selectedComponents[7]{};
			if(ImGui::IsWindowAppearing()) {
				selectedComponents[0] = focusedObject->hasComponent<SpriteComponent>();
				selectedComponents[1] = focusedObject->hasComponent<ArmatureComponent>();
				selectedComponents[2] = focusedObject->hasComponent<ScriptableComponent>();
				selectedComponents[3] = focusedObject->hasComponent<InputControllerComponent>();
				selectedComponents[4] = focusedObject->hasComponent<InteractableComponent>();
				selectedComponents[5] = focusedObject->hasComponent<TriggerComponent>();
				selectedComponents[6] = focusedObject->hasComponent<TraceComponent>();
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
				if(focusedObject->hasComponent<ScriptableComponent>()!=selectedComponents[2]) {
					if(selectedComponents[2]) focusedObject->addComponent<ScriptableComponent>();
					else focusedObject->removeComponent<ScriptableComponent>();
				}
				if(focusedObject->hasComponent<InputControllerComponent>()!=selectedComponents[3]) {
					if(selectedComponents[3]) focusedObject->addComponent<InputControllerComponent>();
					else focusedObject->removeComponent<InputControllerComponent>();
				}
				if(focusedObject->hasComponent<InteractableComponent>()!=selectedComponents[4]) {
					if(selectedComponents[4]) focusedObject->addComponent<InteractableComponent>();
					else focusedObject->removeComponent<InteractableComponent>();
				}
				if(focusedObject->hasComponent<TriggerComponent>()!=selectedComponents[5]) {
					if(selectedComponents[5]) focusedObject->addComponent<TriggerComponent>();
					else focusedObject->removeComponent<TriggerComponent>();
				}
				if(focusedObject->hasComponent<TraceComponent>()!=selectedComponents[6]) {
					if(selectedComponents[6]) focusedObject->addComponent<TraceComponent>();
					else focusedObject->removeComponent<TraceComponent>();
				}

				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if(ImGui::Button("Close", ImVec2(70,0))) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::Checkbox("Sprite##components_manager", &selectedComponents[0]);
			ImGui::Checkbox("Armature##components_manager", &selectedComponents[1]);
			ImGui::Checkbox("Scriptable##components_manager", &selectedComponents[2]);
			ImGui::Checkbox("InputController##components_manager", &selectedComponents[3]);
			ImGui::Checkbox("InteractableComponent##components_manager", &selectedComponents[4]);
			ImGui::Checkbox("TriggerComponent##components_manager", &selectedComponents[5]);
			ImGui::Checkbox("TraceComponent##components_manager", &selectedComponents[6]);

			ImGui::EndPopup();
		}
	}

	void LevelEditor::_menuSave() {
		if(_currentFilePath == "") {
			std::string relative = _getRelativePathFromExplorer("Save world", ".\\Editor\\Saves", "Worlds (*.world)|*.world", true);
			// std::cout<<"--s-"<<relative<<std::endl;
			if(relative != "") {
				try {
					_scenes.saveToFile(relative);
					_currentFilePath = relative;
					_printMenuInfo(std::string("World saved in file: ")+_currentFilePath);
				}
				catch (const std::exception& exc)
				{
					LOG_EXCEPTION(exc);
				}
			}
		}
		else {
			try {
				_scenes.saveToFile(_currentFilePath);
				_printMenuInfo("World saved!");
			}
			catch (const std::exception& exc)
			{
				LOG_EXCEPTION(exc);
			}
		}
	}

	std::string LevelEditor::_getRelativePathFromExplorer(const std::string& title, const std::string& directory, const std::string& filter, bool saveButton) {
		namespace filesystem = std::experimental::filesystem;

		std::string file;
		if(saveButton) file = FileDialog::getSaveFileName(title, directory, filter);
		else file = FileDialog::getOpenFileName(title, directory, filter);
		if(file == "") return "";

		std::string current = filesystem::current_path().string();

		if(current == file.substr(0, current.size())) {
			return file.substr(current.size()+1);
		}

		return "";
	}
  //   std::string LevelEditor::_getRelativeDirectoryPathFromExplorer(const std::string& title, const std::string& directory) {
  //   	namespace filesystem = std::experimental::filesystem;

		// std::string dir;
		// dir = DirectoryDialog::getExistingDirectory(title, directory);
		// if(dir == "") return "";

		// std::string current = filesystem::current_path().string();

		// if(current == dir.substr(0, current.size())) {
		// 	return dir.substr(current.size()+1);
		// }

		// return "";
  //   }
}
