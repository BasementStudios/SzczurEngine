#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

namespace rat {
	class Renderer final {
	private:
		sf::RenderWindow* _window;
		std::vector<const sf::Drawable*> _queue;

	public:
		Renderer(sf::RenderWindow* window);

		void add_to_queue(const sf::Drawable& drawable);
		void draw() const;
	};
}