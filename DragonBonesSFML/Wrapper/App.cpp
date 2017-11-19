#include "App.h"

#include <iostream>

#include "Common.h"

App::App()
{
	_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1024, 768), "Test animacji");
	_window->setFramerateLimit(60);

	//if (!_factory.loadDragonBonesData("zarzadca_ske.json"))
	//if (!_factory.loadDragonBonesData("Sheep_Ani_ske.json"))
	//if (!_factory.loadDragonBonesData("Cedmin_ske.json"))
	if (!_factory.loadDragonBonesData("Cedmin2_ske.json"))
	{
		assert(false);
	}

	//if (!_factory.loadTextureAtlasData("zarzadca_tex.json"))
	//if (!_factory.loadTextureAtlasData("Sheep_Ani_tex.json"))
	//if (!_factory.loadTextureAtlasData("Cedmin_tex.json"))
	if (!_factory.loadTextureAtlasData("Cedmin2_tex.json"))
	{
		assert(false);
	}

	
	_armatureDisplay = std::unique_ptr<dragonBones::SFMLArmatureDisplay>(_factory.buildArmatureDisplay("Armature"));
	//_armatureDisplay = std::unique_ptr<SFMLArmatureDisplay>(_factory.buildArmatureDisplay("armatureName"));
	//_armatureDisplay->getAnimation()->play("WalkCycle");
	//_armatureDisplay->getAnimation()->play("stand", 100);
	_armatureDisplay->getAnimation()->play("Cedmin_bieg");
	//_armatureDisplay->getAnimation()->play("oddech_przerwy", 100);
	//_armatureDisplay->getAnimation()->play("goat_walk_anim");

	//_armatureDisplay->addEvent(dragonBones::EventObject::LOOP_COMPLETE, [] (dragonBones::EventObject *e) { std::cout << "event" << std::endl; });

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

			/*static const float fadeTime = 0.2f;

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
			}*/
		}


		_factory.update(lastUpdate.asSeconds());

		_window->clear(sf::Color::White);

		auto view = _window->getDefaultView();
		view.setCenter(0.f, 0);
		view.zoom(2.f);

		_window->setView(view);
		
		_armatureDisplay->render(*_window);
		_window->display();
	}
}
