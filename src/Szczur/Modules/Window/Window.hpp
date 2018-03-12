#pragma once

/** @file Window.hpp
 ** @description Header file with main class of the Window module. 
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
    Window_t        window;
    sf::VideoMode   videoMode	{1280, 800};
    std::string     title 		{"SzczurEngine"};
	unsigned int	frameRate	{60};

    
    
    /* Properties */
public:
    /** @property Window
     ** @description Provides access to application window.
     ** @access reference get
     **/
    Window_t& getWindow();
    const Window_t& getWindow() const;
    
    /** @property VideoMode
	 ** @description Defines a video mode.
	 ** @access get set
	 **/
	sf::VideoMode getVideoMode() const;
	void setVideoMode(const sf::VideoMode& mode);

	/** @property FrameRate
	 ** @description Limit of updated and rendered frames per second.
	 ** @access get set
	 **/
	unsigned int getFrameRate() const;
	void setFrameRate(const unsigned int limit);

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

	// Disable coping
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;

	// Disable moving
	Window(Window&&) = delete;
	Window& operator = (Window&&) = delete;



	/* Methods */
public:
    // Module system
	void init();
	void input(const sf::Event& event);
	void render();

	/// Cleaning
	void clear(const sf::Color& color = sf::Color::Transparent);

	/// Drawing
	void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
	void draw(const sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);
};



// Module constructor/destructor
template <typename ModulesTuple>
inline Window::Window(ModulesTuple&& tuple) : Module(tuple)
{
	LOG_INFO("[Window] Module initializing");
	this->init();
	LOG_INFO("[Window] Module initialized");
}
inline Window::~Window()
{
	LOG_INFO("[Window] Module destructed");
}

}
