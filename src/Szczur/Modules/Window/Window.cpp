#include "Window.hpp"

/** @file Window.cpp
 ** @description Implementation file with main class of the Window module. 
 ** @author Patryk (Stritch)
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Color.hpp>

#include "Szczur/Utility/Logger.hpp"

namespace rat
{

#ifdef EDITOR
Window* Window::_this;
#endif

/* Properties */
/// Window
Window::Window_t& Window::getWindow()
{
    return this->window;
}
const Window::Window_t& Window::getWindow() const
{
    return this->window;
}

/// VideoMode
sf::VideoMode Window::getVideoMode() const
{
	return this->videoMode;
}
void Window::setVideoMode(const sf::VideoMode& mode)
{
	this->videoMode = mode;
	this->getWindow().create(this->videoMode, this->title);
}

/// FrameRate
unsigned int Window::getFramerateLimit() const
{
	return this->framerateLimit;
}
void Window::setFramerateLimit(const unsigned int limit)
{
	this->framerateLimit = limit;
	this->getWindow().setFramerateLimit(this->framerateLimit);
}

/// Title
const std::string& Window::getTitle() const
{
    return this->title;
}
void Window::setTitle(const std::string& title)
{
    this->title = title;
	this->getWindow().create(this->videoMode, this->title);
}



/* Operators */
/// Constructors
Window::Window()
{
	LOG_INFO(this, ": Window module initializing");
	this->init();
	LOG_INFO(this, ": Window module initialized");
}
/// Deconstructor
Window::~Window()
{
	LOG_INFO(this, ": Window module destructed");
}



/* Methods*/
/// init
void Window::init()
{
	// Create 
    this->setVideoMode(this->videoMode);
	this->getWindow().setFramerateLimit(this->framerateLimit);
	// @todo load from settings
	
#ifdef EDITOR
	// accept dropping files
	DragAcceptFiles(window.getSystemHandle(), true);

	// hook wndproc
	_callback = SetWindowLongPtrW(window.getSystemHandle(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::WndProc));
	
	_this = this;
#endif
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

#ifdef EDITOR
LRESULT CALLBACK Window::WndProc(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam)
{
	switch (Message)
	{
		case WM_DROPFILES:
		{
			std::vector<std::string> files;

			char fileName[1024] = { 0 };

			HDROP hDrop = reinterpret_cast<HDROP>(WParam);

			auto file = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);

			POINT pos;

			DragQueryPoint(hDrop, &pos);

			_this->_lastDropPos = { pos.x, pos.y };

			for (int i = 0; i < file; i++)
			{
				fileName[0] = '\0';

				if (DragQueryFile(hDrop, i, fileName, 1024))
				{
					_this->_droppedFiles.push_back(fileName);
				}
			}

			DragFinish(hDrop);
		} break;
	}

	return CallWindowProc(reinterpret_cast<WNDPROC>(_this->_callback), Handle, Message, WParam, LParam);
}
#endif

}
