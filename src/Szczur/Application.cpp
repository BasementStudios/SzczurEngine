#include "Application.hpp"

namespace rat
{

void Application::init()
{
	_modules.initModule<Window>();
	_modules.initModule<Input>();
	_modules.initModule<Script>();
	_modules.initModule<Shader>();
	_modules.initModule<DragonBones>();
	_modules.initModule<GUI>();
	_modules.initModule<Dialog>();

	#ifdef EDITOR
	{
		ImGui::CreateContext();
		static ImWchar ranges[] = { 0x0020, 0x01FF, 0x0 };
		ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(detail::builtinFontData, detail::builtinFontSize, 16.0f, nullptr, ranges);
		ImGui::SFML::Init(getWindow());
	}
	#endif
	_modules.initModule<MiniWorld>();	
	_modules.getModule<MiniWorld>().init();	
}

void Application::input()
{
	sf::Event event;

	while (getWindow().pollEvent(event)) {
		_modules.getModule<Input>().getManager().processEvent(event);
		_modules.getModule<GUI>().input(event);

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
	[[maybe_unused]] auto deltaTime = _mainClock.restart().asFSeconds();
	
	_modules.getModule<MiniWorld>().update(deltaTime);
	_modules.getModule<GUI>().update();
	_modules.getModule<Dialog>().update();
	_modules.getModule<DragonBones>().update(deltaTime);	
	
	#ifdef EDITOR
	{
		// ImGui::ShowDemoWindow();

		// if (ImGui::Begin("Toolbox", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			// ImGui::Text("%.1f ms", ImGui::GetIO().DeltaTime * 1000.0f);
			// ImGui::Text("%.1f fps", ImGui::GetIO().Framerate);
			// ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
			// ImGui::Checkbox("Shader composer", &_modules.getModule<Shader>().isEditorOpen); // #Stritch
		// }
		// ImGui::End();
	}
	#endif

	_modules.getModule<Shader>().update();

	_modules.getModule<Input>().getManager().finishLogic();
}

void Application::render()
{
	_modules.getModule<Window>().clear({32,32,64});
	
	_modules.getModule<MiniWorld>().render();
	_modules.getModule<GUI>().render();

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
