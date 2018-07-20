#pragma once

#include "Szczur/Utility/Time/Clock.hpp"
#include "Szczur/Modules/PrepScreen/PrepScreen.hpp"
#include "Szczur/Utility/Modules/ModulesHolder.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/AudioEditor/AudioEditor.hpp"
#include "Szczur/Modules/Music/Music.hpp"
#include "Szczur/Modules/Sound/Sound.hpp"
#include "Szczur/Modules/AudioEffects/AudioEffects.hpp"
#include "Szczur/Modules/Equipment/Equipment.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/Dialog/Dialog.hpp"
#include "Szczur/Modules/DialogEditor/DialogEditor.hpp"
#ifdef GUI_TEST
#include "Szczur/Modules/GUITest/GUITest.hpp"
#endif
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/DragonBones/DragonBones.hpp"
#include "Szczur/Modules/World/World.hpp"
#include "Szczur/Modules/Cinematics/Cinematics.hpp"
#include "Szczur/Modules/Listener/Listener.hpp"
#include "Szczur/Modules/Player/Player.hpp"
#include "Szczur/Modules/Battle/Battle.hpp"

#include "ImGuiStyler.hpp"

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
	#ifdef EDITOR
	ImGuiStyler _imGuiStyler;
	#endif
	ModulesHolder<
		Window, 
		Input, 
		Script,
		DragonBones, 
		GUI, 
		World, 
		Dialog,
		DialogEditor,
		Music, 
		Sound, 
		AudioEditor, 
		AudioEffects,
		Cinematics, 
		Listener, 
		Equipment,
		Player,
		Battle, 
		PrepScreen
		#ifdef GUI_TEST
		,GUITest 
		#endif
	> _modules;

};

}
#include "Application.tpp"
