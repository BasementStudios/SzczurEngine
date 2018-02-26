#include "Window.hpp"

namespace rat
{

Window::Window() :
	_window(sf::VideoMode(1280, 720), "SzczurEngine very Alpha", sf::Style::Close)
{
	LOG_INFO(this, " : Module Window constructed");
}

Window::~Window()
{
    LOG_INFO(this, " : Module Window destructed");
}

void Window::clear(const sf::Color& color)
{
    _window.clear(color);
}

void Window::render()
{
    _window.display();
}

Window::Window_t& Window::getWindow()
{
    return _window;
}

const Window::Window_t& Window::getWindow() const
{
    return _window;
}

}
