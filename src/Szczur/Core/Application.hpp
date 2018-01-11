#pragma once

#include <SFML/System/Clock.hpp>

#include "Szczur/Modules/Assets/Assets.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Utility/ModulesHolder.hpp"

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

	template <typename T>
	T& getModule();
	template <typename T>
	const T& getModule() const;

	sf::RenderWindow& getWindow();
	const sf::RenderWindow& getWindow() const;

private:

	ModulesHolder<Assets, Input, Window> _modules;
	sf::Clock _mainClock;

};

}

#include "Application.tpp"
