#pragma once

#include <vector>
#include <variant>

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"

#include "Szczur/Modules/MiniWorld/MiniObject.hpp"

namespace rat {

struct MiniMap
{
	// std::vector<std::unique_ptr<MiniBackgroundObject>> backgroundObjects;
	// std::vector<std::unique_ptr<MiniPathObject>> pathObjects;
	
	std::vector<std::unique_ptr<MiniObject>> objects;
	
	
	Script& script;
	
	MiniMap(Script& script);
	
	// MiniBackgroundObject* newBackgroundObject();
	
	// MiniPathObject* newPathObject();
	
	MiniObject* newObject();
	
	// template <typename T>
	// T* newObject() {		
		// pathObjects.emplace_back(new MiniPathObject);
		// pathObjects.back()->prepare(script);
		// pathObjects.back()->loadScript("scripts/obj_1.lua");
		// return pathObjects.back().get();
	// }
	
	void render(sf::RenderTexture &canvas);
	
	void update();
		
#ifdef EDITOR
	int objectType = 0;
	int selectedObjectId = -1;
	bool objectEditorIsOpen = true;	
	MiniObject* draggedObject;
	MiniObject* hoveredObject;
	MiniObject* selectedObject;
	
	void editorObjectsList();
	void editorObjectEditor();
	void editorDragAndDrop(Window& window, Input& input);
	void editorComponentsChanger();
	void editorDrawObjectFrame(sf::RenderTexture &canvas, MiniObject* object, int state);
	
	//////////////////// EDITOR THINGS ////////////////////	
	void ET_mapPanel();
	void ET_popupOnList(MiniObject* object);
	//////////////////// ---EDITOR THINGS--- ////////////////////
	
#endif

};

}