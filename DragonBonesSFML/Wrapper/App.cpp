#include "App.h"

#include "Common.h"

App::App()
{
	_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1024, 768), "dasd");
	_window->setFramerateLimit(60);

	//if (!_factory.loadDragonBonesData("Armature_ske.json"))
	if (!_factory.loadDragonBonesData("Sheep_Ani_ske.json"))
	{
		assert(false);
	}

	//if (!_factory.loadTextureAtlasData("Armature_tex.json"))
	if (!_factory.loadTextureAtlasData("Sheep_Ani_tex.json"))
	{
		assert(false);
	}

	_armatureDisplay = std::unique_ptr<WrapperArmatureDisplay>(_factory.buildArmatureDisplay("Armature"));
	//_armatureDisplay->getAnimation()->play("WalkCycle");
	_armatureDisplay->getAnimation()->play("goat_walk_anim");
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

			static const float fadeTime = 0.2f;

			if (e.type == sf::Event::KeyPressed)
			{
				switch(e.key.code)
				{
					case sf::Keyboard::Num1:
						_armatureDisplay->getAnimation()->fadeIn("goat_eat_anim", fadeTime);
						break;
					case sf::Keyboard::Num2:
						_armatureDisplay->getAnimation()->fadeIn("goat_sleep_idle_anim", fadeTime);
						break;
					case sf::Keyboard::Num3:
						_armatureDisplay->getAnimation()->fadeIn("goat_trot_anim", fadeTime);
						break;
					case sf::Keyboard::Num4:
						_armatureDisplay->getAnimation()->fadeIn("goat_walk_anim", fadeTime);
						break;
				}
			}
		}

		_factory.update(lastUpdate.asSeconds());

		_window->clear(sf::Color::Black);

		auto view = _window->getDefaultView();
		view.setCenter(0.f, 300.f);

		_window->setView(view);
		
		_armatureDisplay->render(*_window);
		_window->display();
	}
}
