#include "RenderWindow.hpp"

/** @file RenderWindow.cpp
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 ** @author Tomasz (Knayder) Jatkowski
 **/

#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "RenderTarget.hpp"
#include "ShaderProgram.hpp"

namespace sf3d
{

/* Properties */
void RenderWindow::setSize(glm::uvec2 size) 
{
	this->setSize(sf::Vector2u{size.x, size.y});
}



/* Operators */
RenderWindow::RenderWindow() 
{
	;
}

RenderWindow::RenderWindow(
	sf::VideoMode mode,
	const std::string& title,
	ShaderProgram* program,
	sf::Uint32 style,
	const sf::ContextSettings& settings
)
:	sf::RenderWindow(mode, title, style, settings),
	sf3d::RenderTarget(glm::vec2{mode.width, mode.height}, program)
{
	if (!gladLoadGL()) {
		throw std::runtime_error("Failed to initialize GLAD!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, mode.width, mode.height);
}



/* Methods */
void RenderWindow::create(
	sf::VideoMode mode,
	const std::string& title,
	ShaderProgram* program,
	sf::Uint32 style,
	const sf::ContextSettings& settings
)
{
	sf::RenderWindow::create(mode, title, style, settings);
	sf3d::RenderTarget::create(glm::vec2{mode.width, mode.height}, program);
	
	if (!gladLoadGL()) {
		throw std::runtime_error("Failed to initialize GLAD!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, mode.width, mode.height);
}

void RenderWindow::onResize()
{
	const sf::Vector2u size = this->getSize();
	
	if (this->_setActive()) {
		glViewport(0, 0, size.x, size.y);
	}
}

bool RenderWindow::_setActive(bool /*states*/)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

}
