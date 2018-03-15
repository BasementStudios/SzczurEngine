#include "ScriptEditor.hpp"

#include <fstream>
#include <streambuf>

#include <windows.h>
// #include <ShellApi.h>

namespace rat {

	void ScriptEditor::update() {
		
		for(auto& obj : objects) {
			obj->update();
		}
		
		IF_EDITOR {
			ImGui::Begin("Objects", nullptr);
			if(ImGui::Button("ADD OBJECT")) {
				addObject();
				// ShellExecute(GetDesktopWindow(), "open", "./scripts/test.lua", NULL, NULL , SW_SHOW );
			}
			ImGui::SameLine();
			if(ImGui::Button("REMOVE OBJECT")) {
				removeObject(selectedObject);
				// ShellExecute(GetDesktopWindow(), "open", "./scripts/test.lua", NULL, NULL , SW_SHOW );
			}
			
			if(selectedObject!=-1) {		
				auto& obj = objects[selectedObject]->getBase();
				ImGui::Separator();
				ImGui::Text("Script path:");
				
				if(obj.scriptPath=="") {
					ImGui::Text("[no script]");
				}
				else {
					ImGui::Text(obj.scriptPath.c_str());
				}
				if(ImGui::Button("LOAD SCRIPT")) {
					ImGui::OpenPopup("Loading script");
				}
				
                
				if(ImGui::BeginPopupModal("Loading script", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Enter path with script.\n");
					static char buffer[80];
					ImGui::InputText("##NewScript", buffer, 80);
					ImGui::Separator();

					if(ImGui::Button("Load new script", ImVec2(120,0))) { 
						objects[selectedObject]->loadScript(std::string(buffer));
						objects[selectedObject]->init();
						ImGui::CloseCurrentPopup(); 
					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if(ImGui::Button("Cancel", ImVec2(120,0))) { 
						ImGui::CloseCurrentPopup(); 
					}
					ImGui::EndPopup();
				}				
			}
			
			ImGui::Separator();
			for (int i=0; i<objects.size(); ++i) {
				auto obj = objects[i]->getBase();
                if (ImGui::Selectable(obj.name.c_str(), selectedObject == i)) {
					selectedObject = i;
                }
            }
			if(selectedObject>=objects.size()) selectedObject = -1;
			

			ImGui::End();
		}
	}
	
	void ScriptEditor::render() {
		auto& window = getModule<Window>().getWindow();
		canvas.clear(sf::Color::Transparent);
		for(auto& obj : objects) {
			obj->render(canvas);
		}
		canvas.display();
		window.draw(sf::Sprite(canvas.getTexture()));
	}
	
}
