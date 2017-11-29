#include <SFML/Graphics.hpp>

#include "Renderer.hpp"
#include "PreparationScreen.hpp"
#include "Player.hpp"
#include "Elements.hpp"
#include "AssetManager.hpp"


int main() {

	rat::AssetManager::load_sprites();

	sf::RenderWindow window(sf::VideoMode(960, 540), "SFML works!");
	rat::Renderer renderer(&window, rat::AssetManager::BackgroundColor);

	rat::Player p1 = { { 3, 1, 5 }, { 2, 1, 3, 7 } };

	rat::PreparationScreen combat(p1, 2, 3);
	renderer.add_to_queue(combat);

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