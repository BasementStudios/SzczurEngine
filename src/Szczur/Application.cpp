#include "Application.hpp"

#include "Szczur/Modules/DragonBones/UglyArmature.hpp"

namespace rat
{

static UglyArmature* dragon1;
static UglyArmature* cedmin;
static UglyArmature* guard;

void Application::init()
{
	_modules.initModule<Window>();
	_modules.initModule<Input>();
	_modules.initModule<Script>();
	_modules.initModule<Shader>();
	_modules.initModule<DragonBones>();

	#ifdef EDITOR
	{
		ImGui::CreateContext();
		static ImWchar ranges[] = { 0x0020, 0x01FF, 0x0 };
		ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(detail::builtinFontData, detail::builtinFontSize, 16.0f, nullptr, ranges);
		ImGui::SFML::Init(getWindow());
	}
	#endif


	dragon1 = _modules.getModule<DragonBones>().createArmature("Dragon");
	dragon1->setPosition({400, 300});
	dragon1->playAnimation("walk"); // fall, jump, stand, walk
	dragon1->setFlipX(true);

	guard = _modules.getModule<DragonBones>().createArmature("Guard");
	guard->setPosition({1100, 500});
	guard->playAnimation("oddech"); // obrot, oddech

	cedmin = _modules.getModule<DragonBones>().createArmature("Cedmin");
	cedmin->setPosition({500, 500});
	cedmin->playAnimation("Cedmin_Idle_008"); 
	cedmin->setScale({0.34f, 0.34f});

	// Cedmin_Attack_Ending, Cedmin_Attack_Ending_Fast, Cedmin_Attack_Ending_Slow, Cedmin_Attack_Ending_Slow_Frag,
	// Cedmin_Attack_Full_003, Cedmin_Attack_Hit_023, Cedmin_Dab_On_Them, Cedmin_Dodge_3,
	// ...
	// Cedmin_Fight_Idle_011, Cedmin_Fight_Starter_044_sword, Cedmin_Idle_008, Cedmin_Run_051, Cedmin_Second_Hit

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
	[[maybe_unused]] auto deltaTime = _mainClock.restart().asFSeconds();

	#ifdef EDITOR
	{
		ImGui::ShowDemoWindow();

		if (ImGui::Begin("Toolbox", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("%.1f ms", ImGui::GetIO().DeltaTime * 1000.0f);
			ImGui::Text("%.1f fps", ImGui::GetIO().Framerate);
			ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
			ImGui::Checkbox("Shader composer", &_modules.getModule<Shader>().isEditorOpen); // #Stritch
		}
		ImGui::End();
	}
	#endif

	_modules.getModule<DragonBones>().update(deltaTime);

	_modules.getModule<Shader>().update();

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

	//getWindow().draw(*dragon1);
	getWindow().draw(*guard);
	getWindow().draw(*cedmin);



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