#pragma once
 
#include "Szczur/Utility/Time/Clock.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Utility/Modules/ModulesHolder.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/Music/Music.hpp"
#include "Szczur/Modules/AudioEffects/AudioEffects.hpp"
#include "Szczur/Modules/Sound/SoundManager.hpp"
#include "Szczur/Modules/Listener/Listener.hpp"

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

	ModulesHolder<Window, Input, Script, Music, AudioEffects, SoundManager, Listener> _modules;
	sf::Clock _mainClock;

	#ifdef EDITOR
	bool _isImGuiInitialized = false;
	#endif

};

}

#include "Application.tpp"
