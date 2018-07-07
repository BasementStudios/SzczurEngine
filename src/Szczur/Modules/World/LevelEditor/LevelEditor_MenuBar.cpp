#include "LevelEditor.hpp"

#include <experimental/filesystem>

#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include "Szczur/Modules/FileSystem/DirectoryDialog.hpp"

#include <Szczur/Modules/World/World.hpp>

#include <Szczur/ImGuiStyler.hpp>

#include <imgui.h>

#ifdef OS_WINDOWS
#include <shellapi.h>
#endif

namespace rat {


	void LevelEditor::_renderMenuBar() {

        // Create new world
		static bool openModalNew = false;
		static bool openScenesManager = false;

		// Create new project popup
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

		// Scenes manager popup
		if(openScenesManager) {
			if(ImGui::Begin("Scenes Manager##window", &openScenesManager)) {
				
				// Change scene name
				static char name[255];
				std::strcpy(&name[0], _scenes.getCurrentScene()->getName().c_str());
				ImGui::PushItemWidth(-50);
				ImGui::InputText("Name##scene_name", name, 255u);
				ImGui::PopItemWidth();
				_scenes.getCurrentScene()->setName(std::string{name});

				// Calculate width for 3 buttons
				float availWidth = (ImGui::GetContentRegionAvailWidth()-ImGui::GetStyle().ItemSpacing.x-ImGui::GetStyle().WindowPadding.x)*0.33333f;
				
				// Add button
				if(ImGui::Button("Add", ImVec2(availWidth, 0.f))) {
					_scenes.addScene();
				}
				ImGui::SameLine();

				// Remove button
				if(ImGui::Button("Remove", ImVec2(availWidth, 0.f))) {
					_objectsList.unselect();
					_scenes.removeScene(_scenes.getCurrentSceneID());
				}
				ImGui::SameLine();

				// Ok button
				if(ImGui::Button("Ok", ImVec2(availWidth, 0.f))) {
					openScenesManager = false;
				}

				// List of scenes
				if(ImGui::BeginChild("Scenes")) {
					for(auto& it : _scenes.getScenes()) {
						if(ImGui::Selectable(it->getName().c_str(), it->getID() == _scenes.getCurrentSceneID())) {
							_scenes.setCurrentScene(it->getID());
						}
					}
				}
				ImGui::EndChild();
			}
			ImGui::End();	
		}

		// Menu bar
		if(ImGui::BeginMainMenuBar()) {

			// Files
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
				if(!_scenes.isGameRunning()) {
					if(ImGui::MenuItem("Save", "F1")) {
						if (_scenes.menuSave()) {
							printMenuBarInfo(std::string("World saved in file: ") + _scenes.currentFilePath);
						}
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

				ImGui::Separator();

				if(ImGui::MenuItem("Exit")) {
					std::cout << "Exit\n";
				}
				ImGui::EndMenu();
			}

			// Tools
			if(ImGui::BeginMenu("Tools")) {
				ImGui::MenuItem("Objects List", nullptr, &_ifRenderObjectsList);
				ImGui::MenuItem("Sprite Display Data Manager", nullptr, &_ifRenderSpriteDisplayDataManager);
				ImGui::MenuItem("Armature Display Data Manager", nullptr, &_ifRenderArmatureDisplayDataManager);
				ImGui::MenuItem("Dialog Editor", nullptr, &_ifRenderDialogEditor);
				ImGui::MenuItem("Audio Editor", nullptr, &_ifRenderAudioEditor);
				ImGui::EndMenu();
			}

			// Debug
			auto* world = detail::globalPtr<World>;
			if (ImGui::BeginMenu("Debug"))
			{
				if (ImGui::MenuItem("Reload style", nullptr))
				{
					ImGuiStyler::get().reload();
				}
				if (ImGui::MenuItem("Add Player", nullptr))
				{
					_scenes.addPlayer();
				}
				if (ImGui::MenuItem("Add Camera", nullptr))
				{
					_scenes.addCamera();
				}
				if (ImGui::MenuItem("Add Sun", nullptr))
				{
					_scenes.addSun();
				}
				if (ImGui::MenuItem("Enable editor", "F10", world->isEditor()))
				{
					world->setEditor(!world->isEditor());
				}
				if (ImGui::MenuItem("Lock/Unlock camera", "F4"))
				{
					world->getLevelEditor().changeCameraLock();
				}
				if (ImGui::MenuItem("Minecraft camera movement"))
				{
					_isMCCameraMovement = !_isMCCameraMovement;
				}
				if(ImGui::MenuItem("Show demo", nullptr, _ifShowImGuiDemoWindow)) 
				{
					_ifShowImGuiDemoWindow = !_ifShowImGuiDemoWindow;
				}

				ImGui::EndMenu();
			}

			// Play bar
			_renderPlayBar();

			// Menu info
			if(_menuInfoClock.getElapsedTime().asSeconds()<6.0f) {
				ImGui::SameLine(0, 16);
				ImGui::TextColored({0.75f, 0.30f, 0.63f, 1.0f - _menuInfoClock.getElapsedTime().asSeconds()/6.0f}, _menuInfo.c_str());
			}

			ImGui::SameLine(ImGui::GetWindowWidth() - 220);

			if (_isDepthDragging)
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.01f, 0.53f, 0.82f, 1.f)); // cyan
			else
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.42f, 0.0f, 1.f)); // orange

			ImGui::Text((std::string("[Z] Drag move in: ") + (_isDepthDragging ? "depth" : "plane")).c_str());
			ImGui::PopStyleColor();

			ImGui::SameLine(ImGui::GetWindowWidth() - 72);
			ImGui::Text((std::to_string((int)ImGui::GetIO().Framerate)+" FPS").c_str());

		}
		ImGui::EndMainMenuBar();
    }

    void LevelEditor::printMenuBarInfo(const std::string& text) {
		_menuInfo = text;
		_menuInfoClock.restart();
	}

	void LevelEditor::_renderPlayBar() {
		ImGui::SameLine(0.f, 30);
		if(_scenes.isGameRunning()) {				
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Header]);
			ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyle().Colors[ImGuiCol_SeparatorHovered]);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]);

			if(ImGui::Button("Stop##play_bar", ImVec2(60, 0))) {
				_scenes.stopGame();
			}				
			
			ImGui::PopStyleColor(3);
		}
		else {
			if(ImGui::Button("Play##play_bar", ImVec2(60, 0))) {
				_scenes.runGame();
			}
		}
	}
}
