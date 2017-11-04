#include "App.h"

#include "Common.h"

App::App()
{
	_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1024, 768), "dasd");
	_window->setFramerateLimit(60);

	if (!_factory.loadDragonBonesData("dragon_boy_ske.json"))
	{
		assert(false);
	}

	if (!_factory.loadTextureAtlasData("dragon_boy_tex.json"))
	{
		assert(false);
	}

	_armatureDisplay = std::unique_ptr<WrapperArmatureDisplay>(_factory.buildArmatureDisplay("DragonBoy"));
	_armatureDisplay->getAnimation()->play("walk");
	//_armatureDisplay->getAnimation()->stop("walk");
}

App::~App()
{
}


void App::run()
{
	while (_window->isOpen())
	{
		auto lastUpdate = _lastUpdate.restart();

		sf::Event e;

		while (_window->pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				_window->close();
		}

		_factory.update(lastUpdate.asSeconds() / 2);

		_window->clear(sf::Color::White);

		auto view = _window->getDefaultView();
		view.setCenter(0.f, 300.f);

		_window->setView(view);

		_armatureDisplay->render(*_window);
		_window->display();
	}
}
