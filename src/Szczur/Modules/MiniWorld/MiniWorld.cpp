#include "MiniWorld.hpp"

#include "Szczur/Modules/MiniWorld/Components/ComponentColor.hpp"
#include "Szczur/Modules/MiniWorld/MiniObject.hpp"

namespace rat {
	
	MiniWorld::MiniWorld()
	{
		LOG_INFO(this, " : Module MiniWorld constructed");
		auto& window = getModule<Window>().getWindow();
		canvas.create(window.getSize().x, window.getSize().y);
		initScript();
	}
	
	MiniWorld::~MiniWorld()
	{
		LOG_INFO(this, " : Module MiniWorld destructed");
	}
	
	MiniMap* MiniWorld::latestMap() {
		return maps.back().get();
	}
	
	void MiniWorld::newMap() {
		maps.emplace_back(new MiniMap(getModule<Script>()));
	}
	
	void MiniWorld::update() {
		map->update();
		
#ifdef EDITOR
		editor();
		if(openedConsole) editorConsoleForLua(getModule<Input>());
#endif
	}
	
	void MiniWorld::render() {
		canvas.clear(sf::Color::Transparent);
		map->render(canvas);
		canvas.display();
		getModule<Window>().getWindow().draw(sf::Sprite(canvas.getTexture()));
	}
	
	void MiniWorld::init() {
		newMap();
		map = latestMap();
		map->newBackgroundObject();
		map->newPathObject();
	}	
	
	void MiniWorld::initScript() {
		auto module = getModule<Script>().newModule("MiniWorld");
		
		getModule<Script>().initClasses<MiniBackgroundObject, MiniPathObject, MiniObject>();
	}
	
//////////////////////////////////////// EDITOR ////////////////////////////////////////
	
#ifdef EDITOR
	void MiniWorld::editor() {
	
		ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_MenuBar);
		if(ImGui::BeginMenuBar()) {
			ET_menuBar();
		}
		ImGui::EndMenuBar();
		
//////////////////// Main buttons panel ////////////////////

		if(ImGui::Button("Reset")) {
		
		}
		ImGui::SameLine();		
		if(pause) {
			if(ImGui::Button("Resume")) {
				pause = false;
			}			
		}
		else {
			if(ImGui::Button("Play")) {
				pause = true;
			}		
		}		
		ImGui::SameLine();
		ImGui::Checkbox("Console", &openedConsole);
		
		ET_mainPanel();
		
		ImGui::Separator();
		
//////////////////// Map panel ////////////////////

		ImGui::Text("Map:");
		ImGui::SameLine();
		ImGui::Text("Wioska_1");
		if(ImGui::Button("Edit map")) {
			
		}
		ImGui::SameLine();
		if(ImGui::Button("Change map")) {
			
		}
		ImGui::Separator();
		
//////////////////// Objects panel ////////////////////		
		
		map->editorObjectsList();
		
		ImGui::End();
		
//////////////////////////////////////// OBJECT EDITOR ////////////////////////////////////////
		
		map->editorObjectEditor();
		
		map->editorDragAndDrop(getModule<Window>(), getModule<Input>());
		
		ET_outside();
	}
//////////////////////////////////////// CONSOLE ////////////////////////////////////////
	void MiniWorld::editorConsoleForLua(Input& input) {
	
		auto& io = ImGui::GetIO();
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, 5.0f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
		ImGui::SetNextWindowBgAlpha(0.5f);  
		if(openedConsole) {
			ImGui::Begin("Script console", nullptr, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoFocusOnAppearing|ImGuiWindowFlags_NoNav);
			
			static int scriptTarget = 0;
			static int scriptType = 0;
			ImGui::Text("Write lua script here");
			
			// Choose target [Map/Object]
			ImGui::Text("Target:");
			ImGui::RadioButton("Map", &scriptTarget, 0);
			if(map->selectedObject) {	
				ImGui::SameLine();		
				ImGui::RadioButton("Selected object", &scriptTarget, 1);
			}
			
			// Choose scripting type
			ImGui::Text("Type:");			
			ImGui::RadioButton("One-liner", &scriptType, 0);
			ImGui::SameLine();		
			ImGui::RadioButton("Area", &scriptType, 1);
			ImGui::SameLine();		
			ImGui::RadioButton("File", &scriptType, 2);
			ImGui::Separator();
			
			// a) One-liner
			if(scriptType == 0) {
				ImGui::BeginChild("Console story", ImVec2(ImGui::GetWindowContentRegionWidth(), 128), false);
				for(int i = 0; i<consoleStory.size(); ++i) {
					if(i != currConsoleStory) {
						ImGui::Text(consoleStory[i].c_str());
					}
					else {					
						ImGui::TextColored(ImVec4(1.0f,0.0f,1.0f,1.0f), consoleStory[i].c_str());
					}
				}
				ImGui::EndChild();
				ImGui::Separator();
				
				static char buffer[120];			
				if(ImGui::InputText("Input", buffer, 120, ImGuiInputTextFlags_EnterReturnsTrue) && std::string(buffer)!="") {
					if(scriptTarget == 0) {
						
					}
					else if(scriptTarget == 1 && map->selectedObject) {						
						map->selectedObject->runScript(std::string(buffer));
						consoleStory.emplace_back(buffer);
						currConsoleStory = consoleStory.size();
					}
				}
				
				if(ImGui::Button("Clear console story")) {
					consoleStory.clear();
				}
			}
			// b) Area
			else if(scriptType == 1) {
				static char buffer[1024*16];
				ImGui::InputTextMultiline("##ScriptArea", buffer, 1024*16, ImVec2(0.0f, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_AllowTabInput);
				if(ImGui::Button("Run script")) {					
					if(scriptTarget == 0) {
						
					}
					else if(scriptTarget == 1 && map->selectedObject) {						
						map->selectedObject->runScript(std::string(buffer));
					}
				}
			}
			// c) File
			else if(scriptType == 2) {
				static char buffer[120];	
				ImGui::Text("Path to file with script:");
				if(ImGui::InputText("Input##Filepath", buffer, 120, ImGuiInputTextFlags_EnterReturnsTrue)) {				
					if(scriptTarget == 0) {
						
					}
					else if(scriptTarget == 1 && map->selectedObject) {						
						map->selectedObject->runFileScript(std::string(buffer));
					}
				}
			}
			ImGui::End();
		}    
	}
#endif
}