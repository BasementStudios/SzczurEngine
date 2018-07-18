#include "Application.hpp"
#include "Szczur/Utility/Tests.hpp"

#include "Utility/MsgBox.hpp"
#ifdef EDITOR
#	include <imgui.h>
#	include <imgui-SFML.h>
#   include "Szczur/Utility/Debug/NotoMono.ttf.bin"
#endif

namespace rat
{

void Application::init()
{
	LOG_INFO("Initializing modules");

	initModule<Window>();
	initModule<Script>();
	initModule<Input>();
	initModule<Music>();
	initModule<Sound>();
	initModule<AudioEffects>();
	initModule<DragonBones>();
	initModule<World>();
	initModule<GUI>();
	initModule<Dialog>();
	initModule<DialogEditor>();
	initModule<Cinematics>();
	initModule<AudioEditor>();
	initModule<Listener>();
	initModule<Equipment>();
	initModule<Player>();


	LOG_INFO("Modules initialized");

	#ifdef EDITOR
	{
		_imGuiStyler.setPath("Editor/Gui/style.json");
		_imGuiStyler.setupFont();
		ImGui::SFML::Init(getModule<Window>().getWindow());
		_imGuiStyler.reload();

		LOG_INFO("ImGui initialized");
	}
	#endif
	
#ifdef TESTING
	runTests();
#endif
}

void Application::input()
{
	sf::Event event;

	while (getModule<Window>().getWindow().pollEvent(event)) {
		getModule<Input>().getManager().processEvent(event);
		getModule<GUI>().input(event);

		#ifdef EDITOR
		{
			ImGui::SFML::ProcessEvent(event);
		}
		#endif

		if (event.type == sf::Event::Closed) {
			auto result = MsgBox::show(
				getModule<Window>().getWindow().getSystemHandle(), 
				"Do you want to save the world?", "SzczurEngine", 
				MsgBox::Icon::Question, MsgBox::Button::YesNoCancel
			);

			switch (result)
			{
				case MsgBox::Result::Yes:
					getModule<World>().getScenes().menuSave();
					[[fallthrough]];
				case MsgBox::Result::No:
					getModule<Window>().processEvent(event);
					return;
					break;
				case MsgBox::Result::Cancel:
					break;
			}
		}
		else {
			getModule<Window>().processEvent(event);
		}
	}
}

void Application::update()
{
	_imGuiStyler.update();

	auto deltaTime = _mainClock.restart().asFSeconds();
	
	getModule<GUI>().update(deltaTime);
	getModule<Dialog>().update();
	getModule<Music>().update(deltaTime);
	getModule<Equipment>().update(deltaTime);

	#ifdef EDITOR
	{
		ImGui::SFML::Update(getModule<Window>().getWindow(), sf::seconds(deltaTime));
	}
	#endif

	getModule<World>().update(deltaTime);
	getModule<Input>().getManager().finishLogic();
	getModule<Cinematics>().update();
}

void Application::render()
{
	getModule<Window>().clear({24u, 20u, 28u, 255u});

	getModule<World>().render();
	
	getModule<Window>().pushGLStates();
	getModule<GUI>().render();
	getModule<Window>().popGLStates();
	getModule<Cinematics>().render();

	// getModule<World>().render();

	#ifdef EDITOR
	{
		ImGui::SFML::Render(getModule<Window>().getWindow());
	}
	#endif
	
	getModule<Window>().render();
}
int Application::run()
{
	try {
		// Starting
		init();

		LOG_INFO("Starting main loop of application");

		// Main loop
		while (getModule<Window>().getWindow().isOpen()) {
			update();
			render();
			input();
		}

		// Exiting
		#ifdef EDITOR
		{
			ImGui::SFML::Shutdown();
		}
		#endif

		LOG_INFO("Shutdowning application in normal way");
	}
	catch (const std::exception& exception) {
		LOG_EXCEPTION(exception);
		return 1;
	}

	return 0;
}

}
