#include "Application.hpp"

namespace rat
{

#ifdef EDITOR
#   include "Szczur/Utility/Debug/NotoMono.ttf.bin"
#endif

void Application::init()
{
	initModule<Window>();
	initModule<Input>();
	initModule<DragDrop>();

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

		if (event.type == sf::Event::Closed) {
			getModule<Window>().getWindow().close();
		}
	}

	#ifdef EDITOR
	{
		auto& dragDrop = _modules.getModule<DragDrop>();

		if (dragDrop.areFilesDropped())
		{
			auto files = dragDrop.getDroppedFiles();

			auto pos = dragDrop.getLastDropPos();
			LOG_INFO("Mouse pos: ", pos.x, " ", pos.y);
			
			dragDrop.clearDroppedFiles();

			for (auto& file : files)
			{
				LOG_INFO("File: ", file);
			}
		}
	}
	#endif
}

void Application::update()
{
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
		init();

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

	return 0;
}

}
