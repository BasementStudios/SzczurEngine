#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/MiniWorld/ScriptableBase.hpp" 

#ifdef EDITOR
#include "Szczur/Modules/MiniWorld/EditorObject.hpp"
#endif

namespace rat {

struct MiniBackgroundObject
#ifdef EDITOR
: public EditorObject
#endif
{
	//////////////////// EDITOR THINGS ////////////////////
	
	// <<< Put fields and member functions here >>>
	
	void ET_propertiesPanel();
	static void ET_initializeScript(ScriptClass<MiniBackgroundObject>& object); // <--- calling in initScript
	//////////////////// ---EDITOR THINGS--- ////////////////////

	std::string name = "no_name";		
	
	sf::Vector2f pos{0,0};
	sf::Vector2f size{20,20};
	int color{0};	
	
	void init();
	
	void update();
	
	void render(sf::RenderTexture &canvas);
	
	const std::string& getName();
	void setName(const std::string& name);

	const sf::Vector2f& getPosition();	
	void setPosition(const sf::Vector2f& pos);		
	
	/////////////////////////////////////////// SCRIPT ///////////////////////////////////////////
	
	ScriptableBase base;
	sol::function funcInit;
	sol::function funcUpdate;
	
	void loadScript(const std::string& filepath);
	
	void runFileScript(const std::string& filepath);
	
	void runScript(const std::string& code);	
	
	ScriptableBase& getBase();
	
	void prepare(Script& script);
	
	static void initScript(Script& script);	
	
	/////////////////////////////////////////// EDITOR ///////////////////////////////////////////
	
#ifdef EDITOR
	void editor();
	void editor_ObjectFrame(sf::RenderTexture &canvas, int state);
#endif
};

}