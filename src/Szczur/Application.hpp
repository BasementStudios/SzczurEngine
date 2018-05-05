#pragma once

#include "Szczur/Utility/Time/Clock.hpp"
#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Utility/Modules/ModulesHolder.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/World/World.hpp"
#include "Szczur/Modules/DragonBones/DragonBones.hpp"
#include "Szczur/Modules/Camera/Camera.hpp"

namespace rat
{

class Application
{
public:

	// Default constructor
	Application() = default;

	// Disable coping
	Application(const Application&) = delete;
	Application& operator = (const Application&) = delete;

	// Disable moving
	Application(Application&&) = delete;
	Application& operator = (Application&&) = delete;

	// Main functions
	int run();

	void init();

	bool input();

	void update();

	void render();

	template <typename U, typename... Us>
	void initModule(Us&&... args);

	template <typename U>
	U& getModule();
	template <typename U>
	const U& getModule() const;

private:

	Clock _mainClock;
	ModulesHolder<Window, Input, DragonBones, World, Camera> _modules;

};

}

#include "Application.tpp"
