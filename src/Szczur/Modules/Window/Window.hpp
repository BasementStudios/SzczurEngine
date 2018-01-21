#pragma once

#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include "Szczur/Utility/Module.hpp"

namespace rat
{

class Window : public Module<>
{
public:

	using Window_t = sf::RenderWindow;

	template <typename Tuple>
	Window(Tuple&& tuple);

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

template <typename Tuple>
Window::Window(Tuple&& tuple) :
	Module(tuple), _window(sf::VideoMode(1280, 720), "SzczurEngine very Alpha", sf::Style::Close)
{
	LOG_INFO(this, " : Module Window created");
}

}
