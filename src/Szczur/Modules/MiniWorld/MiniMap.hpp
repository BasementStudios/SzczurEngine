#pragma once

#include <vector>
#include <variant>

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"

#include "Szczur/Modules/MiniWorld/MiniObject.hpp"
#include "Szczur/Modules/MiniWorld/Objects/MiniObjectPlayer.hpp"
#include "Szczur/Modules/MiniWorld/Objects/MiniObjectScene.hpp"
#include "Szczur/Modules/MiniWorld/Objects/MiniObjectBackground.hpp"

namespace rat {

struct MiniMap
{
	// std::vector<std::unique_ptr<MiniBackgroundObject>> backgroundObjects;
	// std::vector<std::unique_ptr<MiniPathObject>> pathObjects;
	
	std::unique_ptr<MiniObjectPlayer> player = nullptr;		
	std::vector<std::unique_ptr<MiniObjectScene>> scene;
	std::vector<std::unique_ptr<MiniObjectBackground>> background;
	
	Script& script;
	Window& window;
	Input& input;
	float time = 0;
	bool& pause;
	
	MiniMap(Script& script, Window& window, Input& input, bool& pause);
	
	MiniObjectScene* newScene();
	MiniObjectBackground* newBackground();
	
	MiniObjectPlayer* getPlayer();
	MiniObjectScene* getScene(const std::string& name);
	MiniObjectBackground* getBackground(const std::string& name);
	
	sol::object lua_getSHD(MiniObject*);
	sol::object lua_getSelected();
	sol::object lua_getHovered();
	sol::object lua_getDragged();
	
	void init();
	void update(float deltaTime);
	void render(sf::RenderTexture &canvas);
	static void initScript(Script& script);
		
#ifdef EDITOR
	// int objectType;
	
	MiniObject* selectedObject = nullptr;
	MiniObject* hoveredObject = nullptr;
	MiniObject* draggedObject = nullptr;
	
	int element = -1;
	int group = -1; //0:Single, 1:Scene, 2:Background
	
	void editor();
	void editorRender(sf::RenderTexture& canvas);
	void editorHoveredUpdate(const sf::Vector2f& point);
	void editorDragAndDrop();
	
#endif

};

}