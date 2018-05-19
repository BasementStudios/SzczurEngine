#include "RenderWindow.hpp"
#include <iostream>
namespace sf3d {
	RenderWindow::RenderWindow() {
	}


	RenderWindow::RenderWindow(
		sf::VideoMode mode,
		const std::string & title,
		ShaderProgram* program,
		UINT32 style,
		const sf::ContextSettings & settings
	) :
	sf::RenderWindow(mode, title, style, settings),
	sf3d::RenderTarget(glm::vec2{mode.width, mode.height}, 45.f, program) {
		if(!gladLoadGL()) {
			std::cout << "Failed to initialize GLAD\n";
		}
		glViewport(0, 0, mode.width, mode.height);
	}

	void RenderWindow::create(
		sf::VideoMode mode,
		const std::string & title,
		ShaderProgram* program,
		UINT32 style,
		const sf::ContextSettings & settings
	) {
		sf::RenderWindow::create(mode, title, style, settings);
		if(!gladLoadGL()) {
			std::cout << "Failed to initialize GLAD\n";
		}
		sf3d::RenderTarget::create(glm::vec2{mode.width, mode.height}, 45.f, program);
	}
}
