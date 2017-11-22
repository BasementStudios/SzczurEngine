#include "App.h"

#include <iostream>

#include "Common.h"
#include "SFMLTextureAtlasData.h"

App::App()
{
	_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1024, 768), "Test animacji");
	_window->setFramerateLimit(60);

	auto dragonBonesData = _factory.loadDragonBonesData("Dragon_ske.json");

	auto texturesData = _factory.getTexturesData(dragonBonesData, "Dragon_texture");

	auto textureAtlasData = dragonBones::BaseObject::borrowObject<dragonBones::SFMLTextureAtlasData>();
	textureAtlasData->name = dragonBonesData->name;
	for (auto& textureData : texturesData)
	{
		auto tex = _textureMgr.GetTexture(textureData->path);

		if (tex == nullptr)
			continue;

		textureData->parent = textureAtlasData;
		textureData->setTexture(tex);

		textureAtlasData->addTexture(textureData);
	}

	_factory.addTextureAtlasData(textureAtlasData);

	_armatureDisplay = std::unique_ptr<dragonBones::SFMLArmatureDisplay>(_factory.buildArmatureDisplay("Dragon"));
	_armatureDisplay->getAnimation()->play("walk");
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


		_factory.update(lastUpdate.asSeconds());

		_window->clear(sf::Color::White);

		auto view = _window->getDefaultView();
		view.setCenter(0.f, 0.f);
		view.zoom(2.f);

		_window->setView(view);

		if (_armatureDisplay)
			_armatureDisplay->render(*_window, sf::RenderStates::Default);

		_window->display();
	}
}
