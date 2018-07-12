#pragma once

/** @file Window.hpp
 ** @description Header file with main class of the Window module.
 ** @author Patryk (Stritch)
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>

namespace sf {
	class Drawable;
	class Vertex;
}
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

namespace sf3d {
	class Event;
	class Drawable;
	class Vertex;
	class VertexArray;
}
#include "Szczur/Utility/SFML3D/RenderWindow.hpp"
#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"
#include "Szczur/Utility/SFML3D/ShaderProgram.hpp"
#include "Szczur/Utility/Modules/Module.hpp"

namespace rat {

/// Manages application window.
class Window : public Module<>
{
	/* Types */
public:
	using Window_t = sf3d::RenderWindow;



	/* Variables */
private:
	// Window informations
	sf::VideoMode   videoMode		{1280, 720};

	unsigned int	framerateLimit	{60};

	std::string     title 			{"SzczurEngine"};

	sf::Uint32		windowStyle		{sf::Style::Default};

	// Elmenents for render
	Window_t window;
	
	// Shader programs
	sf3d::ShaderProgram shaderProgram;



	/* Properties */
public:
	/// Provides access to application window.
	Window_t& getWindow() noexcept;
	const Window_t& getWindow() const noexcept;
	
	/// Defines a video mode. Aslo recreates the window.
	sf::VideoMode getVideoMode() const noexcept;
	void setVideoMode(const sf::VideoMode& mode);

	/// Size of window. Related to video mode.
	glm::uvec2 getSize() const noexcept;
	void setSize(glm::uvec2 size);

	/// Limit of updated and rendered frames per second.
	unsigned int getFramerateLimit() const noexcept;
	void setFramerateLimit(unsigned int limit);

	/// Title of application window.
	const std::string& getTitle() const noexcept;
	void setTitle(const std::string& title);

	/// Full screen mode
	bool getFullscreen() const noexcept;
	void setFullscreen(bool state);



	/* Operators */
public:
	Window();
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
	void render();
	void processEvent(sf::Event event);

protected:
	/// Recreates window to apply settings
	void recreateWindow();

public:
	// Cleaning
	void clear(const sf::Color& color = sf::Color::Transparent);

	// GL states
	void pushGLStates();
	void popGLStates();

	// Drawing
	// 	2D
	void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
	void draw(const sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);
	// 	3D
	void draw(const sf3d::Drawable& drawable, const sf3d::RenderStates& states/* = sf3d::RenderStates::Default*/);
	void draw(const sf3d::Drawable& drawable);
	void draw(const sf3d::VertexArray& vertices, const sf3d::RenderStates& states = sf3d::RenderStates::Default);
	void draw(const sf3d::VertexArray& vertices);
};

}
