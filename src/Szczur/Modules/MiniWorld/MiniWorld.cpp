#include "MiniWorld.hpp"

#include "Szczur/Modules/MiniWorld/MiniObject.hpp"

#include "Szczur/Modules/FileSystem/FileDialog.hpp"

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
	
	MiniMap* MiniWorld::getMap() {
		return map;
	}
	
	MiniMap* MiniWorld::latestMap() {
		return maps.back().get();
	}
	
	float MiniWorld::getTime() {
		return map->time;
	}
	
	void MiniWorld::newMap() {
		maps.emplace_back(new MiniMap(getModule<Script>(), getModule<Window>(), getModule<Input>(), pause));
	}
	
	void MiniWorld::update(float deltaTime) {
		map->update(deltaTime);
		
#ifdef EDITOR
		editor();
		if(drawMapEditor) map->editor();
#endif
	}
	
	void MiniWorld::render() {
		canvas.clear(sf::Color::Transparent);
		map->render(canvas);
		
#ifdef EDITOR
		if(drawMapEditor) map->editorRender(canvas);
#endif
		
		canvas.display();
		getModule<Window>().getWindow().draw(sf::Sprite(canvas.getTexture()));
	}
	
	void MiniWorld::init() {
		newMap();
		map = latestMap();
	}	
	
	void MiniWorld::initScript() {
		auto& script = getModule<Script>();
		auto module = getModule<Script>().newModule("MiniWorld");
		
		module.set_function("getMap", &MiniWorld::getMap, this);
		module.set_function("getTime", &MiniWorld::getTime, this);
		script.get().set("World", sol::make_reference(script.get().lua_state(), script.get()["MiniWorld"]));
		
		getModule<Script>().initClasses<MiniMap, MiniObjectScene, MiniObjectBackground, MiniObjectPlayer>();
	}
	
//////////////////////////////////////// EDITOR ////////////////////////////////////////
	
#ifdef EDITOR
	void MiniWorld::editor() {
		ImGui::Begin("Main editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_MenuBar);
		
		if(ImGui::BeginMenuBar()) {
			if(ImGui::BeginMenu("File")) {
				ImGui::MenuItem("Exit");
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("Tools")) {
				ImGui::MenuItem("Shader editor", NULL, &toolShaders);
				ImGui::MenuItem("Console", NULL, &toolConsole);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		
		if(pause) {
			if(ImGui::Button("Play", {60, 0})) {
				pause = false;
			}
		}
		else {			
			if(ImGui::Button("Stop", {60, 0})) {
				pause = true;
			}
		}
		ImGui::Checkbox("Map editor", &drawMapEditor);
		
		ImGui::End();
		
		getModule<Shader>().isEditorOpen = toolShaders;
		
		if(toolConsole) editorToolConsole();
	}
	
	void MiniWorld::editorToolConsole() {
	
		auto& io = ImGui::GetIO();
		// ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, 5.0f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
		// ImGui::SetNextWindowBgAlpha(0.5f);  

		ImGui::Begin("Script console", nullptr, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_AlwaysAutoResize);
		
		static int scriptTarget = 0;
		static int scriptType = 0;
		static std::vector<std::string> consoleStory;
		
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
			ImGui::BeginChild("Console story", ImVec2(ImGui::GetWindowContentRegionWidth(), 96), false);
			for(int i = 0; i<consoleStory.size(); ++i) {
				ImGui::Text(consoleStory[i].c_str());
			}
			ImGui::EndChild();
			ImGui::Separator();
			
			static char buffer[180];			
			if(ImGui::InputText("Input", buffer, 180, ImGuiInputTextFlags_EnterReturnsTrue) && std::string(buffer)!="") {
				if(scriptTarget == 0) {								
					map->runScript(std::string(buffer));
					consoleStory.emplace_back(buffer);
				}
				else if(scriptTarget == 1 && map->selectedObject) {						
					map->selectedObject->runScript(std::string(buffer));
					consoleStory.emplace_back(buffer);
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
					map->runScript(std::string(buffer));
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
			if(ImGui::Button("Load script...", {180, 0})) {
				std::string foundedPath = FileDialog::getOpenFileName("Script", ".");
				if(foundedPath != "") {
					if(scriptTarget == 0) {
						map->runFileScript(foundedPath);
					}
					else if(scriptTarget == 1 && map->selectedObject) {
						map->selectedObject->runFileScript(foundedPath);
					}
				}
			}
		}
		ImGui::End(); 
	}
	
#endif
}