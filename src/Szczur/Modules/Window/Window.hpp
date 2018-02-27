#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include "Szczur/Utility/Modules/Module.hpp"

namespace rat
{

class Window : public Module<>
{
public:

	using Window_t = sf::RenderWindow;

	Window();

	~Window();

	Window(const Window&) = delete;

	Window& operator = (const Window&) = delete;

	Window(Window&&) = delete;

	Window& operator = (Window&&) = delete;

	void clear(const sf::Color& color = sf::Color::Transparent);

	void render();

	Window_t& getWindow();
	const Window_t& getWindow() const;

private:

	Window_t _window;

};

}
