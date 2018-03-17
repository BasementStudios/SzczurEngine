#pragma once

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"

#include "Szczur/Modules/MiniWorld/MiniMap.hpp"

namespace rat {

struct MiniWorld : public Module<Window, Input, Script>
{
	int state = 0;
	
	bool pause = false;

	std::vector<std::unique_ptr<MiniMap>> maps;
	int currentMap;
	MiniMap* map = nullptr; // <<< current map
	
	sf::RenderTexture canvas;
	
	MiniWorld();
	
	~MiniWorld();
	
	MiniMap* latestMap();
	
	void newMap();
	
	void update();
	
	void render();
	
	void init();
	
	void initScript();
	
#ifdef EDITOR
	bool openedConsole = false;
	std::vector<std::string> consoleStory;
	int currConsoleStory = 0;
	static MiniWorld* ptrMiniWorld;
	void editor();
	void editorConsoleForLua(Input& input);
	
	//////////////////// EDITOR THINGS ////////////////////	
	void ET_menuBar();
	void ET_mainPanel();
	void ET_outside();
	//////////////////// ---EDITOR THINGS--- ////////////////////
	
	
#endif
};

}