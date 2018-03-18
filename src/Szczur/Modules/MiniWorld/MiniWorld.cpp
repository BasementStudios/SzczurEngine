#include "MiniWorld.hpp"

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
		maps.emplace_back(new MiniMap(getModule<Script>(), getModule<Window>(), getModule<Input>()));
	}
	
	void MiniWorld::update() {
		map->update();
		
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
		auto module = getModule<Script>().newModule("MiniWorld");
		
		// getModule<Script>().initClasses<>();
	}
	
//////////////////////////////////////// EDITOR ////////////////////////////////////////
	
#ifdef EDITOR
	void MiniWorld::editor() {
		ImGui::Begin("Main editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		
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
	}
#endif
}