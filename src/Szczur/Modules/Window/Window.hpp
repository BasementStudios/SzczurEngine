#pragma once

/** @file Window.hpp
 ** @description Header file with main class of the Window module. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "Szczur/Utility/Module.hpp"
#include "Szczur/Debug/Logger.hpp"

namespace rat {

/** @class Window
 ** @description Manages application window. 
 **/
class Window : public Module<>
{
    using Module::Module;
    


    /* Types */
    using Window_t = sf::RenderWindow;
    


    /* Variables */
private:
    Window_t        _window;
    sf::VideoMode   _currentVideoMode   {1280, 800};
    std::string     _title              {"SzczurEngine"};

    
    
    /* Properties */
public:
    /** @property Window
     ** @description Provides access to application window.
     ** @access get
     **/
    Window_t& getWindow();
    const Window_t& getWindow() const;
    
    /** @property VideoMode
	 ** @description Defines a video mode.
	 ** @access get set
	 **/
	sf::VideoMode getVideoMode() const;
	void setVideoMode(const sf::VideoMode& mode);

    /** @property Title
     ** @description Title of application window.
     ** @access get set
     **/
    const std::string& getTitle() const;
    void setTitle(const std::string& title);



	/* Operators */
public:
	// Module constructor/destructor
	template <typename ModulesTuple>
	Window(ModulesTuple&& tuple);
	~Window();

	// Disable copy operators
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;

	// Disable move operators
	Window(Window&&) = delete;
	Window& operator = (Window&&) = delete;



	/* Methods */
public:
    /// Module system
	void init();
	void input(const sf::Event& event);
	void render();

	/// Window clear
	void clear(const sf::Color& color = sf::Color::Transparent);
};



// Module constructor/destructor
template <typename ModulesTuple>
Window::Window(ModulesTuple&& tuple) :
	Module(tuple), _window(sf::VideoMode(1280, 720), "SzczurEngine very Alpha", sf::Style::Close) // @todo zrobic to z jakiegoś conf albo stałych
{
	LOG_CONSTRUCTOR(); 
	init();
}
inline Window::~Window()
{
    LOG_DESTRUCTOR();
}

}
