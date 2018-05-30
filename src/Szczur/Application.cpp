#include "Application.hpp"

#include "Utility/MsgBox.hpp"

namespace rat
{

void Application::init()
{
	LOG_INFO("Initializing modules");

	initModule<Window>();
	initModule<Script>();
	initModule<Input>();
	// initModule<AudioEditor>();
	// initModule<Music>("Assets/Music/");
	// initModule<AudioEffects>();
	initModule<DragonBones>();
	initModule<Camera>();
	initModule<World>();
	// initModule<GUI>();
	// initModule<Dialog>();
	// initModule<DialogEditor>();

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
}

bool Application::input()
{
	sf::Event event;

	while (getModule<Window>().getWindow().pollEvent(event)) {
		getModule<Input>().getManager().processEvent(event);
		// getModule<GUI>().input(event);

		#ifdef EDITOR
		{
			ImGui::SFML::ProcessEvent(event);
		}
		#endif

		if (event.type == sf::Event::Closed) {
			getModule<Window>().getWindow().close();
			return false;
		}
	}
	return true;
}

void Application::update()
{
	_imGuiStyler.update();

	[[maybe_unused]] auto deltaTime = _mainClock.restart().asFSeconds();
	// getModule<Dialog>().update();
	// getModule<GUI>().update(deltaTime);
	// getModule<Music>().update(deltaTime);


	/*
		Put other updates here
	*/
	#ifdef EDITOR
	{
		ImGui::SFML::Update(getModule<Window>().getWindow(), sf::seconds(deltaTime));
	}
	#endif

	getModule<DragonBones>().update(deltaTime);
	getModule<World>().update(deltaTime);
	getModule<Camera>().update();
	getModule<Input>().getManager().finishLogic();
}

void Application::render()
{
	glEnable(GL_DEPTH_TEST);
	getModule<Window>().getWindow().clear(22.f, 20.f, 28.f, 255.f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	getModule<World>().render();

	// getModule<Window>().pushGLStates();
	// getModule<GUI>().render();
	// getModule<Window>().popGLStates();

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
		init();

		LOG_INFO("Starting main loop of application");

		while (getModule<Window>().getWindow().isOpen()) {
			if(input()) {
				update();
				render();
			}
		}
	}
	catch (const std::exception& exception) {
		LOG_EXCEPTION(exception);
	}

	#ifdef EDITOR
	{
		ImGui::SFML::Shutdown();
	}
	#endif

	LOG_INFO("Shutdowning application");

	return 0;
}

}