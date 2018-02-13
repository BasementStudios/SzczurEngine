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

/* Methods */
/// init
void Canvas::init()
{
	// Set target (it will aslo recreate layers to fit the target)
	this->setTarget(this->getModule<Window>().getWindow());
}

/// input
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

/// render
void Canvas::render()
{
	// Invoke `display` from RenderCanvas.
	this->display();
}

}
