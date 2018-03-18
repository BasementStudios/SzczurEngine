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
	
	MiniMap(Script& script, Window& window, Input& input);
	
	MiniObjectScene* newScene();
	MiniObjectBackground* newBackground();
	
	void init();
	void update(float deltaTime);
	void render(sf::RenderTexture &canvas);	
		
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