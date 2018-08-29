#include "Application.hpp"
#include "Szczur/Utility/Tests.hpp"

#include "Utility/MsgBox.hpp"

#include "Szczur/Utility/Debug/ExceptionHandler.hpp"

namespace rat
{

void Application::init()
{
	exc::init();

	LOG_INFO("Initializing modules");

	initModule<Window>();
	initModule<Input>();
	initModule<DragonBones>();
	initModule<World>();

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

		#ifdef EDITOR
		{
			ImGui::SFML::ProcessEvent(event);
		}
		#endif

		getModule<Window>().processEvent(event);
	}
}

void Application::update()
{
	_imGuiStyler.update();

	auto deltaTime = _mainClock.restart().asFSeconds();

	getModule<DragonBones>().update(deltaTime);

	#ifdef EDITOR
	{
		ImGui::SFML::Update(getModule<Window>().getWindow(), sf::seconds(deltaTime));
	}
	#endif

	getModule<Input>().getManager().finishLogic();
}

void Application::render()
{
	getModule<Window>().clear({24.f/255.f, 20.f/255.f, 28.f/255.f, 1.f});

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
