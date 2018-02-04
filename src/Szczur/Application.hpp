#pragma once

#include <SFML/System/Clock.hpp>

#include "Szczur/Utility/ModulesHolder.hpp"
#include "Szczur/Modules/Assets/Assets.hpp"
#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"

namespace rat
{

class Application
{
public:

	Application() = default;

	Application(const Application&) = delete;
	Application& operator = (const Application&) = delete;

	Application(Application&&) = delete;
	Application& operator = (Application&&) = delete;

	int run();

	void init();

	void input();

	void update();

	void render();

	sf::RenderWindow& getWindow();
	const sf::RenderWindow& getWindow() const;

	/// @test Testing Assets module
	sf::Texture* tex;
	sf::Music* mus;

private:

	ModulesHolder<Assets, Input, Window, Canvas> _modules;
	sf::Clock _mainClock;

};

}
