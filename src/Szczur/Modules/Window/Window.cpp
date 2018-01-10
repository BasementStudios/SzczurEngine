#include "Window.hpp"

/** @file Window.hpp
 ** @description Header file with main class of the Window module. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "Szczur/Debug/Logger.hpp"

namespace rat
{

// init
void Window::init()
{
	LOG_INFO("Window: Initializing");
	this->setVideoMode(this->_currentVideoMode);
	LOG_INFO("Window: Initialized!");
}

// input
void Window::input(const sf::Event& event)
{
	;
}

// render
void Window::render()
{
	this->getWindow().display();
}

// Window
Window::Window_t& Window::getWindow()
{
	return this->_window;
}
const Window::Window_t& Window::getWindow() const
{
	return this->_window;
}

// VideoMode
sf::VideoMode Window::getVideoMode() const
{
	return this->_currentVideoMode;
}
void Window::setVideoMode(const sf::VideoMode& mode)
{
	this->_currentVideoMode = mode;
	this->getWindow().create(this->_currentVideoMode, this->_title);
}

// Title
const std::string& Window::getTitle() const
{
	return this->_title;
}
void Window::setTitle(const std::string& title)
{
	this->_title = title;
}

}
