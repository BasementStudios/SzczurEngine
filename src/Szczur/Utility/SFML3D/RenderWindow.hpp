#pragma once

#include <string>

#include <SFML/Window.hpp> // @todo . remove (for Uint32 now)
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/Window/ContextSettings.hpp>

namespace sf3d {
	class ShaderProgram;
}
#include "RenderTarget.hpp"

namespace sf3d
{

class RenderWindow : public sf::RenderWindow, public RenderTarget {
public:
	RenderWindow();
	RenderWindow(
		sf::VideoMode mode, 
		const std::string& title, 
		ShaderProgram* program = nullptr,
		sf::Uint32 style = sf::Style::Default, 
		const sf::ContextSettings& settings = sf::ContextSettings{24u, 0u, 0u, 3u, 3u}
	);
	RenderWindow(const RenderWindow&) = delete;
	void operator=(const RenderWindow&) = delete;

	void create(
		sf::VideoMode mode, 
		const std::string& title, 
		ShaderProgram* program = nullptr,
		sf::Uint32 style = sf::Style::Default,
		const sf::ContextSettings& settings = sf::ContextSettings{24u, 0u, 0u, 3u, 3u}
	);

	/// Updates RenderWindow (glViewport) after resizing
	void onResize() override;

	virtual bool _setActive(bool states = true) override;
	
	using sf3d::RenderTarget::clear;
	using sf3d::RenderTarget::draw;
	using sf::RenderWindow::draw;
};

}
