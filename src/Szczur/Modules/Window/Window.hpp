#pragma once

#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "Szczur/Utility/Modules.hpp"

namespace rat
{

class Window : public Module<>
{
public:

	using Window_t = sf::RenderWindow;

public:

	template <typename Tuple>
	Window(Tuple&& tuple) :
		Module(tuple),
		_window(sf::VideoMode(1280, 720), "SzczurEngine very Alpha", sf::Style::Close)
	{
		LOG_INFO(this, " -> Module Window created");
	}

	~Window()
	{
		LOG_INFO(this, " -> Module Window destructed");
	}

	Window(const Window&) = delete;

	Window& operator = (const Window&) = delete;

	Window(Window&&) = delete;

	Window& operator = (Window&&) = delete;

	void clear(const sf::Color& color = sf::Color::Transparent)
	{
		_window.clear(color);
	}

	void render()
	{
		_window.display();
	}

	Window_t& getWindow()
	{
		return _window;
	}

	const Window_t& getWindow() const
	{
		return _window;
	}

private:

	Window_t _window;

};

}
