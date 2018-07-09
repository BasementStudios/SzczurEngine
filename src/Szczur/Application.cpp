#include "Application.hpp"

namespace rat
{

#ifdef EDITOR
#   include "Szczur/Utility/Debug/NotoMono.ttf.bin"
#endif

void Application::init()
{
	LOG_INFO("Initializing modules");

	initModule<Window>();
	initModule<Input>();

	LOG_INFO("Modules initialized");

	#ifdef EDITOR
	{
		ImGui::CreateContext();
		static ImWchar ranges[] = { 0x0020, 0x01FF, 0x0 };
		ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(builtinFontData, builtinFontSize, 16.0f, nullptr, ranges);
		ImGui::SFML::Init(getModule<Window>().getWindow());
	}
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
	auto deltaTime = _mainClock.restart().asFSeconds();


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
