#include "Window.hpp"

/** @file Window.cpp
 ** @description Implementation file with main class of the Window module.
 ** @author Patryk (Stritch)
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>
#include <memory> // unique_ptr
#include <stdexcept>

#include <glad/glad.h>

#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include "Szczur/Utility/SFML3D/Drawable.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"
#include "Szczur/Utility/SFML3D/Vertex.hpp"
#include "Szczur/Utility/SFML3D/Shader.hpp"
#include "Szczur/Utility/SFML3D/ShaderProgram.hpp"
#include "Szczur/Utility/Logger.hpp"

namespace rat
{

/* Properties */
// Window
Window::Window_t& Window::getWindow() noexcept
{
	return this->window;
}
const Window::Window_t& Window::getWindow() const noexcept
{
	return this->window;
}

// VideoMode
sf::VideoMode Window::getVideoMode() const noexcept
{
	return this->videoMode;
}
void Window::setVideoMode(const sf::VideoMode& mode)
{
	this->videoMode = mode;
	LOG_INFO("VideoMode: { width: ", this->videoMode.width,  ", height: ", this->videoMode.height, ", bitsPerPixel: ", this->videoMode.bitsPerPixel, " }");

	this->recreateWindow();
}

// Size
glm::uvec2 Window::getSize() const noexcept
{
	return {this->videoMode.width, this->videoMode.height};
}
void Window::setSize(glm::uvec2 size)
{
	this->videoMode.width = size.x;
	this->videoMode.height = size.y;
	this->setVideoMode(this->videoMode);
}

// FrameRate
unsigned int Window::getFramerateLimit() const noexcept
{
	return this->framerateLimit;
}
void Window::setFramerateLimit(unsigned int limit)
{
	this->framerateLimit = limit;
	LOG_INFO("framerateLimit: ", this->framerateLimit);

	this->getWindow().setFramerateLimit(this->framerateLimit);
}

// Title
const std::string& Window::getTitle() const noexcept
{
	return this->title;
}
void Window::setTitle(const std::string& title)
{
	this->title = title;
	LOG_INFO("Window title: ", this->title);

	this->getWindow().setTitle(this->title);
}

// Fullscreen
bool Window::getFullscreen() const noexcept
{
	return this->windowStyle & sf::Style::Fullscreen;
}
void Window::setFullscreen(bool state)
{
	if (state) {
		this->windowStyle = sf::Style::Fullscreen;
	}
	else {
		this->windowStyle = sf::Style::Default;
	}
	this->recreateWindow();
}



/* Operators */
// Constructors
Window::Window()
:	window(this->videoMode, this->title)
{
	LOG_INFO("Window module initializing");
	this->init();
	LOG_INFO("Window module initialized");
}
// Destructor
Window::~Window()
{
	LOG_INFO("Window module destructed");
}



/* Methods*/
// init
void Window::init()
{
	try {
		// Set video mode, also creates window
		//  Create window MUST be first, before messing around with GL because of dynamic GL/GLAD functions binding,
		//  which require some rendering context which is created in SFML `sf::RenderWindow::create`.
		this->setVideoMode(this->videoMode); // @todo ? load videomode from settings

		// Print OpenGL version
		LOG_INFO("OpenGL version: ", GLVersion.major, ".", GLVersion.minor);

		// GL flags
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CCW);

		// Shader
		try {
			sf3d::Shader VShader(sf3d::Shader::Vertex, "Assets/Shaders/assemble.vert");
			sf3d::Shader FShader(sf3d::Shader::Fragment, "Assets/Shaders/assemble.frag");

			this->shaderProgram.linkShaders(VShader, FShader);
			LOG_INFO("Shader loaded, compiled and linked.");
		}
		catch (...) {
			std::throw_with_nested(std::runtime_error("Shader couldn't been loaded."));
		}

		this->getWindow().setDefaultShaderProgram(this->shaderProgram);
	}
	catch (...) {
		std::throw_with_nested(std::runtime_error("Cannot initialized Window module."));
	}
}

// render
void Window::render()
{
	//glDisable(GL_DEPTH_TEST);
	this->getWindow().display();
	//glEnable(GL_DEPTH_TEST);
}

// processEvent
void Window::processEvent(sf::Event event)
{
	switch (event.type) {
		case sf::Event::Resized:
		{
			// Update video mode
			this->videoMode.width = event.size.width;
			this->videoMode.height = event.size.height;
		}
		break;
		
		case sf::Event::Closed:
		{
			this->getWindow().close();
		}
		break;
		
		default:
		{
			// Do nothing.
		}
		break;
	}
}

// Window recreate
void Window::recreateWindow()
{
	this->getWindow().create(this->videoMode, this->title, &(this->shaderProgram), this->windowStyle);
	this->setFramerateLimit(this->framerateLimit);
}

// clear
void Window::clear(const sf::Color& color)
{
	this->getWindow().clear(color, GL_COLOR_BUFFER_BIT);
	//this->getWindow().clear(color, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// GL states
void Window::pushGLStates()
{
	this->getWindow().pushGLStates();
}
void Window::popGLStates()
{
	this->getWindow().popGLStates();
}

// draw
// 2D
void Window::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
	this->getWindow().draw(drawable, states);
}
void Window::draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states)
{
	this->getWindow().draw(vertices, vertexCount, type, states);
}
// 3D
void Window::draw(const sf3d::Drawable& drawable, const sf3d::RenderStates& states)
{
	this->getWindow().draw(drawable, states);
}
void Window::draw(const sf3d::Drawable& drawable)
{
	this->getWindow().draw(drawable);
}
void Window::draw(const sf3d::VertexArray& vertices, const sf3d::RenderStates& states)
{
	this->getWindow().draw(vertices, states);
}
void Window::draw(const sf3d::VertexArray& vertices)
{
	this->getWindow().draw(vertices);
}

}
