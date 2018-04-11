#include "RenderWindow.hpp"

namespace sf3d {
	RenderWindow::RenderWindow(
		sf::VideoMode mode,
		const std::string & title,
		UINT32 style,
		const sf::ContextSettings & settings,
		const char* vertexPath,
		const char* fragmentPath
	) :
		sf::RenderWindow(mode, title, style, settings),
		sf3d::RenderTarget(vertexPath, fragmentPath, glm::vec2{mode.width, mode.height}, 45.f)
	{
		if(!gladLoadGL()) {
			std::cout << "Failed to initialize GLAD\n";
			while(true);
		}
		glViewport(0, 0, mode.width, mode.height);
	}
}
