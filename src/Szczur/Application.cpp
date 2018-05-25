#include "Application.hpp"

#include "Utility/MsgBox.hpp"

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
	initModule<Script>();
	initModule<GUI>();
	initModule<Dialog>();
	initModule<DialogEditor>();

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
		getModule<GUI>().input(event);

		#ifdef EDITOR
		{
			ImGui::SFML::ProcessEvent(event);
		}
		#endif

		if (event.type == sf::Event::Closed) {

			if (getModule<DialogEditor>().isProjectLoaded())
			{
				auto result = MsgBox::show(getModule<Window>().getWindow().getSystemHandle(), "Do you want to save the project??", "Dialog Editor", MsgBox::Icon::Question, MsgBox::Button::YesNoCancel);

				switch (result)
				{
					case MsgBox::Result::Yes:
						getModule<DialogEditor>().saveProject();
					case MsgBox::Result::No:
						getModule<Window>().getWindow().close();
						break;
					case MsgBox::Result::Cancel:
						break;
				}
			}
			else
			{
				getModule<Window>().getWindow().close();
			}
		}
	}
}

void Application::update()
{
	[[maybe_unused]] auto deltaTime = _mainClock.restart().asFSeconds();
	getModule<Dialog>().update();
	getModule<GUI>().update(deltaTime);

	/*
		Put other updates here
	*/

	#ifdef EDITOR
	{
		ImGui::SFML::Update(getModule<Window>().getWindow(), sf::seconds(deltaTime));

		getModule<DialogEditor>().update();
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
	getModule<GUI>().render();

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
			input();
			update();
			render();
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
