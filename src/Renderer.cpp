#include "Renderer.hpp"

namespace rat
{
	Renderer::Renderer(sf::RenderWindow* window, const sf::Color& clear)
		: _window(window), _clear(clear) {
	}

	void Renderer::add_to_queue(const sf::Drawable& drawable) {
		_queue.push_back(&drawable);
	}

	void Renderer::draw() const {
		_window->clear(_clear);

		for (auto& drawable : _queue)
		{
			_window->draw(*drawable);
		}

		_window->display();
	}
}