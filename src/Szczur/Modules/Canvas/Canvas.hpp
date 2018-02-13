#pragma once

/** @file Canvas.hpp
 ** @description Header file with main class of the Canvas module. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>						// string
#include <array>						// array
#include <memory>						// unique_ptr

#include <SFML/Window/Event.hpp>

#include "Szczur/Utility/Module.hpp"
#include "Szczur/Debug/Logger.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "RenderLayer.hpp"
#include "RenderCanvas.hpp"

namespace rat
{

/** @enum Layers
 ** @description List of layers IDs.
 ** @info Done inside main `rat` naming space instead of packing into `rat::Canvas` as shorthand.
 **/
enum class Layers : std::size_t
{
	Background, 	First = Background,
	Scene, 
	Foreground,
	GUI,			Last = GUI,
	Count
};



/** @class Canvas
 ** @description Manages the render layers, draws them on the window.
 **/
class Canvas : public RenderCanvas<(std::size_t)Layers::Count>, public Module<Window>
{	
	using Module::Module;



	/* Operators */
public:
	/// Module constructor/destructor
	template <typename ModulesTuple>
	Canvas(ModulesTuple&& tuple);
	~Canvas();

	// Disable copy operators
	Canvas(const Canvas&) = delete;
	Canvas& operator = (const Canvas&) = delete;

	// Disable move operators
	Canvas(Canvas&&) = delete;
	Canvas& operator = (Canvas&&) = delete;



	/* Methods */
public:
	/// Module system
	void init();
	void input(const sf::Event& event);
	void render();
};



/// Module constructor/destructor
template <typename ModulesTuple>
Canvas::Canvas(ModulesTuple&& tuple) :
	Module(tuple)
{
	LOG_INFO("[Canvas] Module initializing");
	init();
	LOG_INFO("[Canvas] Module initialized!");
}
inline Canvas::~Canvas()
{
	LOG_INFO("[Canvas] Module descructed");
}

}
