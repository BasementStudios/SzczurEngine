#pragma once
 
#include "Szczur/Utility/Time/Clock.hpp"
#include "Szczur/Utility/Modules/ModulesHolder.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Script/Script.hpp"

#include "Szczur/Modules/ScriptEditor/ScriptEditor.hpp"
 
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

private:

	ModulesHolder<Window, Input, Script, ScriptEditor> _modules;
	Clock _mainClock;

};

}