#pragma once

/** @file World.hpp
 ** @description Header file with main class of the World module. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <vector>
#include <string>
#include <unordered_map>
#include "Szczur/Utility/SFML3D/RenderStates.hpp"
#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Camera/Camera.hpp"
#include "Szczur/Modules/Input/Input.hpp"
//#include "Szczur/Modules/DragonBones/DragonBones.hpp"

#include "Szczur/Modules/World/Componable.hpp"
#include "BaseObject.hpp"
#include "DisplayData/SpriteDisplayData.hpp" /*
#include "DisplayData/SpriteStatesDisplayData.hpp"
#include "DisplayData/ArmatureDisplayData.hpp"
#include "DisplayData/ArmaturePosesDisplayData.hpp" */
#include "Components/SpriteComponent.hpp" 
#include "Components/InputController.hpp" /*
#include "Components/SpriteStatesComponent.hpp"
#include "Components/ArmatureComponent.hpp"
#include "Components/ArmaturePosesComponent.hpp" */

namespace rat {

/** @class World
 ** @description Manages objects in the game world through the maps system, including loading and saving, object types management and other.
 **/
class World : public Module<Window, Input, Camera/*, DragonBones*/>
{
    /* Types */
	#ifdef EDITOR
		BaseObject* _focusedObject{nullptr};
		BaseObject* _editorCamera{nullptr};

		bool _ifRenderObjectsList{true};
		bool _ifRenderDisplayDataManager{false};

		void _renderDisplayDataManager();
		void _renderFocusedObjectsParams();
		void _renderObjectsList();
		void _renderBar();
	#endif
	BaseObject* _camera{nullptr};
protected:
	using ObjectsHolder_t = Componable::ObjectsHolder<
		// Base type of object
		BaseObject,
		// All variants // @todo . budowanie z listy typów komponentów;
		//Componable::Object<BaseObject>,
		Componable::Object<BaseObject, SpriteComponent>
		//Componable::Object<BaseObject, InputController>,
		//Componable::Object<BaseObject, SpriteComponent, InputController>
		/*,
		Componable::Object<BaseObject, SpriteStatesComponent>,
		Componable::Object<BaseObject, ArmatureComponent>,
		Componable::Object<BaseObject, ArmaturePosesComponent> */
	>;



	/* Fields */
public: // @warn priv&prop
    std::vector<SpriteDisplayData> 			spriteDisplayData; /*
	std::vector<SpriteStatesDisplayData>	spriteStatesDisplayData;
	std::vector<ArmatureDisplayData> 		armatureDisplayData;
	std::vector<ArmaturePosesDisplayData> 	armaturePosesDisplayDatas; */

	std::unordered_map<std::string, ObjectsHolder_t> objects;/*{
		{"Foreground", ObjectsHolder_t{}},
		{"Background", ObjectsHolder_t{}},
		{"Path", ObjectsHolder_t{}},
		{"Single", ObjectsHolder_t{}}
	};*/

	//ObjectsHolder_t	singlesObjects;
	//ObjectsHolder_t	pathObjects;
	//ObjectsHolder_t	backgroundObjects;
	//ObjectsHolder_t	foregroundObjects;

	std::string currentMap {""};



	/* Operators */
public:
	// Module constructor/destructor
	World();
	~World();

	// Disable coping
	World(const World&) = delete;
	World& operator = (const World&) = delete;

	// Disable moving
	World(World&&) = delete;
	World& operator = (World&&) = delete;



	/* Methods */
public:
    // Module system
	void init();
	void update(float deltaTime);
	void render();

	// @todo . load
	// @todo . save
};

}
