#pragma once

#include <memory>

#include <SFML\Graphics.hpp>

#include "WrapperFactory.h"
#include "TextureMgr.h"
#include "WrapperArmatureDisplay.h"

class App
{
private:
	WrapperFactory								_factory;
	std::unique_ptr<WrapperArmatureDisplay>		_armatureDisplay;

	TextureMgr									_textureMgr;

	std::unique_ptr<sf::RenderWindow>			_window;

	sf::Clock									_lastUpdate;

public:
	App();
	~App();

	void run();
};

