#pragma once

#include <memory>

#include <SFML\Graphics.hpp>

#include "SFMLFactory.h"
#include "SFMLArmatureDisplay.h"

#include "TextureMgr.h"

class App
{
private:
	dragonBones::SFMLFactory							_factory;
	std::unique_ptr<dragonBones::SFMLArmatureDisplay>	_armatureDisplay;

	//Armature											armature;

	TextureMgr											_textureMgr;

	std::unique_ptr<sf::RenderWindow>					_window;

	sf::Clock											_lastUpdate;

public:
	App();
	~App();

	void run();
};

