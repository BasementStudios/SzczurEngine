#include "Application.hpp"
 
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>


namespace rat
{

void Application::init()
{
	_modules.initModule<Window>();
	_modules.initModule<Input>();
	_modules.initModule<Script>();

	IF_EDITOR {
		ImGui::CreateContext();
		// static ImWchar ranges[] = { 0x0020, 0x01FF, 0x0 };
		// ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(detail::builtinFontData, detail::builtinFontSize, 16.0f, nullptr, ranges);
		static const ImWchar ranges[] =
		{
			0x0020, 0x00FF,
			0x0100, 0x01FF,
			0
		};
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("font.ttf", 20.f, nullptr, ranges);		
		
		ImGui::SFML::Init(getWindow());
	}
	
	_modules.initModule<ScriptEditor>();
	
	_modules.getModule<ScriptEditor>().init();	
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

		detail::setVar("time", _mainClock.getElapsedTime().asFSeconds());

		ImGui::Begin("Toolbox", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("%.1f ms", ImGui::GetIO().DeltaTime * 1000.0f);
		ImGui::Text("%.1f fps", ImGui::GetIO().Framerate);
		ImGui::Text("%.1f time", detail::getVar<float>("time"));
		ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
		// magic
		ImGui::End();
	}

	_modules.getModule<ScriptEditor>().update();
	_modules.getModule<Input>().getManager().finishLogic();
}

void Application::render()
{
	_modules.getModule<Window>().clear(sf::Color{ 64, 96, 64 });

	IF_EDITOR {
		getWindow().resetGLStates();
		ImGui::SFML::Render(getWindow());
	}

	_modules.getModule<ScriptEditor>().render();
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