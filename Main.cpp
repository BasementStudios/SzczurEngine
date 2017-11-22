#include <SFML/Graphics.hpp>

#include "Renderer.hpp"
#include "PreparationScreen.hpp"

int main() {
	sf::RenderWindow window(sf::VideoMode(960, 540), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	rat::Renderer renderer(&window);
	renderer.add_to_queue(shape);

	rat::PreparationScreen combat(1, 2);

	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		combat.run();

		renderer.draw();
	}

	return 0;
}