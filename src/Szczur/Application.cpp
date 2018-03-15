#include "Application.hpp"

#include <SFML/Graphics.hpp>

namespace rat
{

void Application::init()
{
	_modules.initModule<Window>();
	_modules.initModule<Input>();
	_modules.initModule<SPFX>();

	#ifdef EDITOR
	{
		ImGui::CreateContext();
		static ImWchar ranges[] = { 0x0020, 0x01FF, 0x0 };
		ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(detail::builtinFontData, detail::builtinFontSize, 16.0f, nullptr, ranges);
		ImGui::SFML::Init(getWindow());
		gVar->create<sf::Shader*>("test_shader", nullptr); // #Stritch
	}
	#endif
}

void Application::input()
{
	sf::Event event;

	while (getWindow().pollEvent(event)) {
		_modules.getModule<Input>().getManager().processEvent(event);

		#ifdef EDITOR
		{
			ImGui::SFML::ProcessEvent(event);
		}
		#endif

		if (event.type == sf::Event::Closed) {
			getWindow().close();
		}
	}

	#ifdef EDITOR
	{
		static Clock editorClock;
		ImGui::SFML::Update(getWindow(), editorClock.restart().asSfTime());
	}
	#endif
}

void Application::update()
{
	// [[maybe_unused]] auto deltaTime = _mainClock.restart().asFSeconds();

	#ifdef EDITOR
	{
		ImGui::ShowDemoWindow();

		if (ImGui::Begin("Toolbox", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("%.1f ms", ImGui::GetIO().DeltaTime * 1000.0f);
			ImGui::Text("%.1f fps", ImGui::GetIO().Framerate);
			ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
			ImGui::Checkbox("Shader composer", &_modules.getModule<SPFX>()._isEditorOpen); // #Stritch
		}
		ImGui::End();
	}
	#endif

	_modules.getModule<SPFX>().update();

	_modules.getModule<Input>().getManager().finishLogic();
}

void Application::render()
{
	_modules.getModule<Window>().clear();

	#ifdef EDITOR
	{
		ImGui::SFML::Render(getWindow());
	}
	#endif

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

	#ifdef EDITOR
	{
		ImGui::SFML::Shutdown();
	}
	#endif

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
