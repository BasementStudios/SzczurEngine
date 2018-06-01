#include "LevelEditor.hpp"

#include <experimental/filesystem>

#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include "Szczur/Modules/FileSystem/DirectoryDialog.hpp"

#include <Szczur/ImGuiStyler.hpp>

#include <ImGui/imgui.h>

#include <shellapi.h>

namespace rat {


	void LevelEditor::_renderMenuBar() {

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
				_scenes.currentFilePath = "";
				_scenes.removeAllScenes();
				_scenes.setCurrentScene(_scenes.addScene()->getID());
				_objectsList.unselect();
				printMenuBarInfo(std::string("New world created!"));
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
			}
			ImGui::SameLine();
			if(ImGui::Button("Ok")) {
				ImGui::CloseCurrentPopup();
			}
			if(ImGui::BeginChild("Scenes")) {
				for(auto& it : _scenes.getScenes()) {
					if(ImGui::Selectable(it->getName().c_str(), it->getID() == _scenes.getCurrentSceneID())) {
						_scenes.setCurrentScene(it->getID());
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
					std::string relative = _scenes.getRelativePathFromExplorer("Load world", ".\\Editor\\Saves", "Worlds (*.world)|*.world");
					// std::cout<<"--l-"<<relative<<std::endl;
					if(relative != "") {
						try {
							_scenes.loadFromFile(relative);
							_scenes.currentFilePath = relative;
							printMenuBarInfo(std::string("World loaded from file: ")+_scenes.currentFilePath);
						}
						catch (const std::exception& exc)
						{
							LOG_EXCEPTION(exc);
						}
					}
				}
				if(ImGui::MenuItem("Save", "F1")) {
					_scenes.menuSave();
                    printMenuBarInfo(std::string("World saved in file: ")+_scenes.currentFilePath);
				}
				if(ImGui::MenuItem("Save As")) {
					std::string relative = _scenes.getRelativePathFromExplorer("Save world", ".\\Editor\\Saves", "Worlds (*.world)|*.world", true);
					// std::cout<<"--s-"<<relative<<std::endl;
					if(relative != "") {
						try {
							_scenes.saveToFile(relative);
							_scenes.currentFilePath = relative;
							printMenuBarInfo(std::string("World saved in file: ")+_scenes.currentFilePath);
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
				ImGui::MenuItem("Sprite Display Data Manager", nullptr, &_ifRenderSpriteDisplayDataManager);
				ImGui::MenuItem("Armature Display Data Manager", nullptr, &_ifRenderArmatureDisplayDataManager);
				ImGui::MenuItem("Dialog Editor", nullptr, &_ifRenderDialogEditor);
				ImGui::MenuItem("Audio Editor", nullptr, &_ifRenderAudioEditor);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Debug"))
			{
				if (ImGui::MenuItem("Reload style", nullptr))
				{
					ImGuiStyler::get().reload();
				}
				if (ImGui::MenuItem("Add player", nullptr))
				{
					_scenes.addPlayer();
				}
				if(ImGui::MenuItem("Show demo", nullptr, _ifShowImGuiDemoWindow)) {
					_ifShowImGuiDemoWindow = !_ifShowImGuiDemoWindow;
					LOG_INFO(_ifShowImGuiDemoWindow)
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

    void LevelEditor::printMenuBarInfo(const std::string& text) {
		_menuInfo = text;
		_menuInfoClock.restart();
	}
}
