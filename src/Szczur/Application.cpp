#include "Application.hpp"

#include <SFML/Graphics.hpp>

namespace rat
{

void Application::init()
{
	_modules.initModule<Window>();
	_modules.initModule<Input>();

	IF_EDITOR {
		ImGui::CreateContext();
        static ImWchar ranges[] = { 0x0020, 0x01FF, 0x0 };
        ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(detail::builtinFontData, detail::builtinFontSize, 16.0f, nullptr, ranges);
        ImGui::SFML::Init(getWindow());
	}
}

void Application::input()
{
	sf::Event event;

	while (getWindow().pollEvent(event)) {
		_modules.getModule<Input>().getManager().processEvent(event);

		IF_EDITOR {
			ImGui::SFML::ProcessEvent(event);
		}

		if (event.type == sf::Event::Closed) {
			getWindow().close();
		}
	}

	IF_EDITOR {
		static Clock editorClock;
		ImGui::SFML::Update(getWindow(), editorClock.restart().asSfTime());
	}
}

void Application::update()
{
	IF_EDITOR {
		ImGui::ShowDemoWindow();
	}

	_modules.getModule<Input>().getManager().finishLogic();
}

void Application::render()
{
	_modules.getModule<Window>().clear();

	IF_EDITOR {
		getWindow().resetGLStates();
		ImGui::SFML::Render(getWindow());
	}

	_modules.getModule<Window>().render();
}

int Application::run()
{
	init();

	while (getWindow().isOpen()) {
		input();
		update();
		render();
	}

	IF_EDITOR {
		ImGui::SFML::Shutdown();
	}

	return 0;
}

sf::RenderWindow& Application::getWindow()
{
	return _modules.getModule<Window>().getWindow();
}

const sf::RenderWindow& Application::getWindow() const
{
	return _modules.getModule<Window>().getWindow();
}

}
