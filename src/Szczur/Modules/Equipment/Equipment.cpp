#include "Equipment.hpp"
namespace rat {
	Equipment::Equipment() {

		auto& mainWindow = getModule<Window>();
		auto& window = mainWindow.getWindow();

		mainWindow.pushGLStates();
		_canvas.create(window.getSize().x, window.getSize().y);
		mainWindow.popGLStates();

		init();
	}

	void Equipment::init() {
		auto& gui = getModule<GUI>();

		_base = gui.addInterface();
		_base->setSize(100, 100);
	}

	void Equipment::update(float deltaTime) {

	}

	void Equipment::render() {
		auto& mainWindow = getModule<Window>();

		mainWindow.pushGLStates();

		_canvas.clear(sf::Color::Transparent);
		_canvas.display();

		mainWindow.getWindow().draw(sf::Sprite(_canvas.getTexture()));

		mainWindow.popGLStates();
	}
}