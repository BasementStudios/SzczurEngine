#include "Application.hpp"

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/FileSystem/FileDialog.hpp"

namespace rat
{

void Application::init()
{
	_modules.initModule<Window>();
	_modules.initModule<Input>();
	
	/*auto files = FileDialog::getOpenFileNames("Wybierz pliki");

	for (auto& file : files)
	{
		LOG_INFO("File: ", file);
	}*/

	auto file = FileDialog::getOpenFileName("Wybierz plik: ", "D:\\", "Images (*.png, *.jpg)|*.png;*.jpg|Music (*.mp3)|*.mp3");

	if (!file.empty())
	{
		LOG_INFO("File: ", file);
	}
}

void Application::input()
{
	sf::Event event;

	while (getWindow().pollEvent(event)) {
		_modules.getModule<Input>().getManager().processEvent(event);

		if (event.type == sf::Event::Closed) {
			getWindow().close();
		}
	}
}

void Application::update()
{
	_modules.getModule<Input>().getManager().finishLogic();
}

void Application::render()
{
	_modules.getModule<Window>().clear();
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
