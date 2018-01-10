#pragma once

/** @file Canvas.hpp
 ** @description Header file with main class of the Canvas module. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>						// string
#include <array>						// array
#include <memory>						// unique_ptr

#include <SFML/Window/Event.hpp>

#include "Szczur/Utility/Modules.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "RenderCanvas.hpp"

namespace rat
{

enum class Layers : std::size_t
{
	Background, 	First = Background,
	SceneBack, 
	Objects, 
	SceneFront, 
	GUI,			Last = GUI,
	Count
};

/** @class Canvas
 ** @description Manages the render layers, draws
 **/
class Canvas : public RenderCanvas<(std::size_t)Layers::Count>, public Module<Window>, Module<>::Inputable, Module<>::Renderable
{	
	using Module::Module;



	/* Methods */
public:
	/// Module init
	void init();

	/// Module input
	void input(const sf::Event& event);
};

}
