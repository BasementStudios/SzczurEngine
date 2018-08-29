#pragma once

/** @file RenderWindow.hpp
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 ** @author Tomasz (Knayder) Jatkowski
 **/

#include <string>

#include <glm/vec2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/Window/ContextSettings.hpp>

namespace sf3d {
	class ShaderProgram;
}
#include "RenderTarget.hpp"
#include "ContextSettings.hpp"

namespace sf3d
{

// OpenGL version configuration
inline constexpr unsigned int OpenGL_MajorVersion = 3u;
inline constexpr unsigned int OpenGL_MinorVersion = 3u;

// Default settings contexts for SFML and 3D
inline const sf::ContextSettings DefaultContextSettingsSFML = sf::ContextSettings{24u, 8u, 0u, OpenGL_MajorVersion, OpenGL_MinorVersion};
inline const sf3d::ContextSettings DefaultContextSettingsSFML3D = sf3d::ContextSettings::Default;

/// Provides RenderTarget that is presented as window in the system.
class RenderWindow
	: public sf::RenderWindow, public RenderTarget
{
	/* Properties */
public:
	void setSize(glm::uvec2 size);
	using sf::RenderWindow::setSize; // Vector2u

private:
	// Used for GLADGLLoader, which makes gladLoadGL before anything else
	struct GLADGLLoader {} myGLADGLLoader;


	
	/* Operators */
public:
	RenderWindow();
	RenderWindow(
		sf::VideoMode mode, 
		const std::string& title, 
		const sf3d::ContextSettings& contextSettingsSFML3D = DefaultContextSettingsSFML3D,
		ShaderProgram* program = nullptr,
		sf::Uint32 style = sf::Style::Default, 
		const sf::ContextSettings& settings = DefaultContextSettingsSFML
	);
	
	/// Non-copyable
	RenderWindow(const RenderWindow&) = delete;
	void operator = (const RenderWindow&) = delete;



	/* Methods */
public:
	void create(
		sf::VideoMode mode, 
		const std::string& title, 
		const sf3d::ContextSettings& contextSettingsSFML3D = DefaultContextSettingsSFML3D,
		ShaderProgram* program = nullptr,
		sf::Uint32 style = sf::Style::Default,
		const sf::ContextSettings& settings = DefaultContextSettingsSFML
	);

	/// Updates RenderWindow (glViewport) after resizing
	void onResize() override;

	virtual bool setActive(bool states = true) override;
	
	using sf3d::RenderTarget::clear;
	using sf3d::RenderTarget::draw;
	using sf::RenderWindow::draw;
};

}
