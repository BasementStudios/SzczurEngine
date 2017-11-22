#include "App.h"

#include <iostream>

#include "Common.h"

App::App()
<<<<<<< Updated upstream
=======
	//: armature("Armature", "Sheep_Ani_ske.json", "Sheep_Ani_tex.json")
>>>>>>> Stashed changes
{
	_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1024, 768), "Test animacji");
	_window->setFramerateLimit(60);

<<<<<<< Updated upstream
	//if (!_factory.loadDragonBonesData("zarzadca_ske.json"))
	//if (!_factory.loadDragonBonesData("Sheep_Ani_ske.json"))
	//if (!_factory.loadDragonBonesData("Cedmin_ske.json"))
	if (!_factory.loadDragonBonesData("Cedmin2_ske.json"))
=======

	//armature.play("goat_walk_anim");

	//if (!_factory.loadDragonBonesData("zarzadca_ske.json"))
	//auto dragonBonesData = _factory.loadDragonBonesData("NewProject_ske.json");
	/*auto dragonBonesData = _factory.loadDragonBonesData("NewProject_ske.json");
	if (!dragonBonesData)
	{
		assert(false);
	}

	_factory.loadTextures(dragonBonesData, "NewProject_texture");*/

	//_factory.loadTextureAtlasData("Dragon_tex.json");

	if (!_factory.loadDragonBonesData("Sheep_Ani_ske.json"))
	{
		assert(false);
	}

	if (!_factory.loadTextureAtlasData("Sheep_Ani_tex.json"))

	{
		assert(false);
	}

	/*//if (!_factory.loadDragonBonesData("DragonBoy_ske.json"))
	//if (!_factory.loadDragonBonesData("Cedmin_ske.json"))
	/if (!_factory.loadDragonBonesData("Cedmin2_ske.json"))
>>>>>>> Stashed changes
	{
		assert(false);
	}

	//if (!_factory.loadTextureAtlasData("zarzadca_tex.json"))
	//if (!_factory.loadTextureAtlasData("Sheep_Ani_tex.json"))
<<<<<<< Updated upstream
	//if (!_factory.loadTextureAtlasData("Cedmin_tex.json"))
	if (!_factory.loadTextureAtlasData("Cedmin2_tex.json"))
	{
		assert(false);
	}
=======
	//if (!_factory.loadTextureAtlasData("Cedmin2_tex.json"))
	
	{
		assert(false);
	}*/
>>>>>>> Stashed changes

	
	_armatureDisplay = std::unique_ptr<dragonBones::SFMLArmatureDisplay>(_factory.buildArmatureDisplay("Armature"));
	//_armatureDisplay = std::unique_ptr<SFMLArmatureDisplay>(_factory.buildArmatureDisplay("armatureName"));
	//_armatureDisplay->getAnimation()->play("WalkCycle");
<<<<<<< Updated upstream
	//_armatureDisplay->getAnimation()->play("stand", 100);
	_armatureDisplay->getAnimation()->play("Cedmin_bieg");
	//_armatureDisplay->getAnimation()->play("oddech_przerwy", 100);
	//_armatureDisplay->getAnimation()->play("goat_walk_anim");

=======

	//_armatureDisplay->getAnimation()->play("walk");
//	_armatureDisplay->getAnimation()->play("animtion0");
	//_armatureDisplay->getAnimation()->play("oddech_przerwy", 100);
	_armatureDisplay->getAnimation()->play("goat_walk_anim");
	
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
		view.setCenter(0.f, 0);
=======
		view.setCenter(0.f, 0.f);
>>>>>>> Stashed changes
		view.zoom(2.f);

		_window->setView(view);
		
<<<<<<< Updated upstream
		_armatureDisplay->render(*_window);
=======
		//armature.render(*_window);
		if (_armatureDisplay)
			_armatureDisplay->render(*_window, sf::RenderStates::Default);
>>>>>>> Stashed changes
		_window->display();
	}
}
