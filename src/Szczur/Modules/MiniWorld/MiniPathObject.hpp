#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/MiniWorld/ScriptableBase.hpp"

#ifdef EDITOR
#include "Szczur/Modules/MiniWorld/EditorObject.hpp"
#endif

namespace rat {
	
struct MiniPathObject 
#ifdef EDITOR
: public EditorObject
#endif
{
	//////////////////// EDITOR THINGS ////////////////////
	
	// <<< Put fields and member functions here >>>
	
	void ET_propertiesPanel(); // <--- calling in editor
	static void ET_initializeScript(ScriptClass<MiniPathObject>& object); // <--- calling in initScript
	//////////////////// ---EDITOR THINGS--- ////////////////////
	
	std::string name = "no_name";	
	
	sf::Vector2f pos{40,40};
	sf::Vector2f size{30,30};
	sf::Vector2f colliderRelPos{10,10};
	sf::Vector2f colliderSize{40,10};
	bool centerCollider = false;	
	
	void renderCollider(sf::RenderTexture &canvas);

	void init();
	
	void update();	
	
	void render(sf::RenderTexture &canvas);
	
	const std::string& getName();
	void setName(const std::string& _name);
	
	const sf::Vector2f& getPosition();	
	void setPosition(const sf::Vector2f&);		
	
	/////////////////////////////////////////// SCRIPT ///////////////////////////////////////////
	
	ScriptableBase base;
	sol::function funcInit;
	sol::function funcUpdate;
	sol::function funcAction;
	
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