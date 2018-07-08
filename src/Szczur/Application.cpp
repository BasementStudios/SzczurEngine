#include "Application.hpp"

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

		#ifdef EDITOR
		{
			ImGui::SFML::ProcessEvent(event);
		}
		#endif

		getModule<Window>().processEvent(event);
	}

	return true;
}

void Application::update()
{
	_imGuiStyler.update();

	[[maybe_unused]] auto deltaTime = _mainClock.restart().asFSeconds();

	/*
		Put other updates here
	*/

	#ifdef EDITOR
	{
		ImGui::SFML::Update(getModule<Window>().getWindow(), sf::seconds(deltaTime));

		/*
			Put main editor window here
		*/
	}
	#endif

	getModule<Input>().getManager().finishLogic();
}

void Application::render()
{
	getModule<Window>().clear();

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
