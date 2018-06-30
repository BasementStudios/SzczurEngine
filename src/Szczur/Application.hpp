#pragma once
 
#include "Szczur/Utility/Time/Clock.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Utility/Modules/ModulesHolder.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/QuestJournal/QuestJournal.hpp"

#ifdef GUI_TEST
#include "Szczur/Modules/GUITest/GUITest.hpp"
#endif
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/QuestLog/QuestLog.hpp"
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

	ModulesHolder<Window, Input, Script, GUI, QuestJournal, QuestLog
	#ifdef GUI_TEST
	,GUITest 
	#endif
	> _modules;
	Clock _mainClock;
	#ifdef EDITOR
	bool _isImGuiInitialized = false;
	#endif

};

}
#include "Application.tpp"