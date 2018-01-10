#include "Canvas.hpp"

/** @file Canvas.cpp
 ** @description Implementation file for main class of the Canvas module.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <tuple>						// tuple, get
#include <string>						// string
#include <array>						// array
#include <memory>						// unique_ptr

#include <SFML/Window/Event.hpp>

#include "Szczur/Debug/Logger.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "RenderLayer.hpp"

namespace rat
{

// init
void Canvas::init()
{
	LOG_INFO("Canvas: Initializing");
	this->setTarget(this->_getModule<Window>().getWindow());
	LOG_INFO("Canvas: Initialized!");
}

// input
void Canvas::input(const sf::Event& event)
{
	switch(event.type) {
		case sf::Event::EventType::Resized:
			this->setSize(event.size.width, event.size.height);
			break;
		// @warn @test Window::setVideoMode 
		default: break;
	}
}

}
