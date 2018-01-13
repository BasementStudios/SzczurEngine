#pragma once

#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "Szczur/Utility/Module.hpp"

namespace rat
{

class Window;

template <typename T>
class _Window : public Module<Window, T>
{
public:

	using Window_t = sf::RenderWindow;

public:

	_Window() :
		_window(sf::VideoMode(1280, 720), "SzczurEngine very Alpha", sf::Style::Close)
	{
		LOG_INFO(this, " -> Module Window created");
	}

	~_Window()
	{
		LOG_INFO(this, " -> Module Window destructed");
	}

	_Window(const _Window&) = delete;

	_Window& operator = (const _Window&) = delete;

	_Window(_Window&&) = delete;

	_Window& operator = (_Window&&) = delete;

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

REGISTER_GAME_MODULE(Window, _Window);

}
