#include "Window.hpp"

/** @file Window.cpp
 ** @description Implementation file with main class of the Window module. 
 ** @author Patryk (Stritch)
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "Szczur/Debug/Logger.hpp"

namespace rat
{

/* Properties */
/// Window
Window::Window_t& Window::getWindow()
{
    return this->_window;
}
const Window::Window_t& Window::getWindow() const
{
    return this->_window;
}

/// VideoMode
sf::VideoMode Window::getVideoMode() const
{
	return this->_currentVideoMode;
}
void Window::setVideoMode(const sf::VideoMode& mode)
{
	this->_currentVideoMode = mode;
	this->getWindow().create(this->_currentVideoMode, this->_title);
}

/// Title
const std::string& Window::getTitle() const
{
    return this->_title;
}
void Window::setTitle(const std::string& title)
{
    this->_title = title;
}



/* Methods*/
/// init
void Window::init()
{
	// Create 
    this->setVideoMode(this->_currentVideoMode);
}

/// input
void Window::input(const sf::Event& event)
{
	;
}

/// render
void Window::render()
{
	this->getWindow().display();
}

/// clear
void Window::clear(const sf::Color& color)
{
    this->getWindow().clear(color);
}

void Window::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
	this->getWindow().draw(drawable, states);
}

void Window::draw(const sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states)
{
	this->getWindow().draw(vertices, vertexCount, type, states); 
}

}
