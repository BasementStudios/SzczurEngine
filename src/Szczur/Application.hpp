#pragma once

#include "Szczur/Utility/Time/Clock.hpp"
#include "Szczur/Utility/Modules/ModulesHolder.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/DragonBones/DragonBones.hpp"
#include "Szczur/Modules/World/World.hpp"
#include "Szczur/Modules/Camera/Camera.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/Dialog/Dialog.hpp"
#include "Szczur/Modules/DialogEditor/DialogEditor.hpp"

namespace rat
{

class Application
{
public:

	///
	Application() = default;

	///
	Application(const Application&) = delete;

	///
	Application& operator = (const Application&) = delete;

	///
	Application(Application&&) = delete;

	///
	Application& operator = (Application&&) = delete;

	///
	~Application() = default;

	///
	void init();

	///
	bool input();

	///
	void update();

	///
	void render();

	///
	int run();

	///
	template <typename U, typename... Us>
	void initModule(Us&&... args);

	///
	template <typename U>
	U& getModule();

	///
	template <typename U>
	const U& getModule() const;

private:


	Clock _mainClock;
	ModulesHolder<Window, Input, Script, GUI, Dialog, DragonBones, World, Camera, DialogEditor> _modules;

};

}

#include "Application.tpp"
