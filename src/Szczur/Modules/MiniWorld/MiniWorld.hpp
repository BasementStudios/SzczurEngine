#pragma once

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Shader/Shader.hpp"

#include "Szczur/Modules/MiniWorld/MiniMap.hpp"

namespace rat {

struct MiniWorld : public Module<Window, Input, Script, Shader>
{
	int state = 0;
	
	bool pause = false;
	

	std::vector<std::unique_ptr<MiniMap>> maps;
	int currentMap;
	MiniMap* map = nullptr; // <<< current map
	
	sf::RenderTexture canvas;
	sf::RenderTexture editorCanvas;
	
	MiniWorld();
	
	~MiniWorld();
	
	MiniMap* getMap();
	MiniMap* latestMap();
	float getTime();
	
	void newMap();
	
	void update(float deltaTime);
	
	void render();
	
	void init();
	
	void initScript();
	
#ifdef EDITOR
	bool toolShaders = false;
	Shader* shader;
	bool toolConsole = false;	
	void editorToolConsole();
	
	bool drawMapEditor = false;
	void editor();
#endif
};

}