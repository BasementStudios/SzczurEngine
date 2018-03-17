#pragma once

#include <vector>
#include <variant>

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/Input/Input.hpp"

#ifdef EDITOR
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/MiniWorld/EditorObject.hpp"
#endif

#include "Szczur/Modules/MiniWorld/MiniBackgroundObject.hpp"
#include "Szczur/Modules/MiniWorld/MiniPathObject.hpp"

#include "Szczur/Modules/MiniWorld/MiniObject.hpp"

namespace rat {

struct MiniMap
{
	std::vector<std::unique_ptr<MiniBackgroundObject>> backgroundObjects;
	std::vector<std::unique_ptr<MiniPathObject>> pathObjects;
	
	std::vector<std::unique_ptr<MiniObject>> objects;
	
	Script& script;
	
	MiniMap(Script& script);
	
	MiniBackgroundObject* newBackgroundObject();
	
	MiniPathObject* newPathObject();
	
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
	EditorObject* draggedObject;
	EditorObject* hoveredObject;
	EditorObject* selectedObject;
	
	void editorObjectsList();
	void editorObjectEditor();
	void editorDragAndDrop(Window& window, Input& input);
	
	//////////////////// EDITOR THINGS ////////////////////	
	void ET_mapPanel();
	void ET_popupOnList(EditorObject* object);
	//////////////////// ---EDITOR THINGS--- ////////////////////
	
#endif
	
};

}