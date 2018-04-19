#pragma once

#include "RenderTarget.hpp"
#include "RenderStates.hpp"
#include <SFML/Graphics.hpp>
#include "ShaderProgram.hpp"

namespace sf3d {
	class RenderWindow : public sf::RenderWindow, public RenderTarget {
	public:
		RenderWindow();
		RenderWindow(const RenderWindow&) = delete;
		void operator=(const RenderWindow&) = delete;
		RenderWindow(
			sf::VideoMode mode, 
			const std::string& title, 
			ShaderProgram* program = nullptr,
			UINT32 style = sf::Style::Default,
			const sf::ContextSettings& settings = sf::ContextSettings{24u, 0u, 0u, 3u, 3u}
		);

		void create(
			sf::VideoMode mode, 
			const std::string& title, 
			ShaderProgram* program = nullptr,
			UINT32 style = sf::Style::Default,
			const sf::ContextSettings& settings = sf::ContextSettings{24u, 0u, 0u, 3u, 3u}
		);


		//void clear(const sf::Color&) = delete;
		//const sf::View& getView() const = delete;
		//const sf::View& getDefaultView() const = delete;
		//void setView(const sf::View&) = delete;

		using sf3d::RenderTarget::clear;
		using sf3d::RenderTarget::draw;
		using sf3d::RenderTarget::getView;
		using sf3d::RenderTarget::getDefaultView;
		using sf3d::RenderTarget::setView;
		using sf::RenderWindow::draw;
	};
}