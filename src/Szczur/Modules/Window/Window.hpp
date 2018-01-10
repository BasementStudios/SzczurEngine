#pragma once

/** @file Window.hpp
 ** @description Header file with main class of the Window module. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "Szczur/Utility/Modules.hpp"

namespace rat {

/** @class Window
 ** @description Manages application window. 
 **/
class Window : public Module<>, Module<>::Inputable, Module<>::Renderable 
{
	using Module::Module;
	


	/* Types */
	using Window_t = sf::RenderWindow;
	


	/* Variables */
private:
	Window_t        _window;
	sf::VideoMode   _currentVideoMode   {1280, 800};
	std::string     _title              {"SzczurEngine"};

	
	
	/* Methods */
public:
	/// Module init
	void init();

	/// Module input
	void input(const sf::Event& event);

	/// Module render
	void render();
	
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
};

}
