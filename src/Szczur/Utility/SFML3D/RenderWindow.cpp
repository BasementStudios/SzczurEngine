#include "RenderWindow.hpp"
#include <iostream>
namespace sf3d {
	RenderWindow::RenderWindow(
		sf::VideoMode mode,
		const std::string & title,
		ShaderProgram* program,
		UINT32 style,
		const sf::ContextSettings & settings
	) :
	sf3d::RenderTarget(glm::vec2{mode.width, mode.height}, 45.f, program),
	sf::RenderWindow(mode, title, style, settings) {
		if(!gladLoadGL()) {
			std::cout << "Failed to initialize GLAD\n";
			while(true);
		}
		glViewport(0, 0, mode.width, mode.height);
	}
}
