#include "Application.hpp"

#include "Utility/MsgBox.hpp"
#ifdef EDITOR
#	include <imgui.h>
#	include <imgui-SFML.h>
#   include "Szczur/Utility/Debug/NotoMono.ttf.bin"
#endif

namespace rat
{

void Application::init()
{
	LOG_INFO("Initializing modules");

	initModule<Window>();
	initModule<Script>();
	initModule<Input>();
	initModule<Music>();
	initModule<Sound>();
	initModule<AudioEffects>();
	initModule<DragonBones>();
	initModule<World>();
	initModule<Cinematics>();
	initModule<AudioEditor>();
	initModule<Listener>();
	initModule<GUI>();
	initModule<Player>();
	#ifdef GUI_TEST
	{
		initModule<GUITest>();
	}
	#endif
	initModule<Battle>();

	initModule<PrepScreen>();
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

	// getModule<Battle>().loadBattle("Assets/Battles/battle_1.json");

}

bool Application::input()
{
	sf::Event event;

	while (getModule<Window>().getWindow().pollEvent(event)) {
		getModule<Window>().processEvent(event);
		getModule<Input>().getManager().processEvent(event);

		#ifdef GUI_TEST
		getModule<GUITest>().input(event);
		#endif

		#ifdef EDITOR
		{
			ImGui::SFML::ProcessEvent(event);
		}
		#endif

		/*
			Put other inputs here
		*/		
		// getModule<Battle>().input(event);

		// if (event.type == sf::Event::Resized) {
		// 	auto windowSize = getModule<Window>().getWindow().getSize();
		// 	getModule<Window>().setVideoMode(sf::VideoMode(event.size.width, event.size.height));
		// }

		if (event.type == sf::Event::Closed) {
			auto result = MsgBox::show(getModule<Window>().getWindow().getSystemHandle(), "Do you want to save the world?", "SzczurEngine", MsgBox::Icon::Question, MsgBox::Button::YesNoCancel);

			switch (result)
			{
				case MsgBox::Result::Yes:
					getModule<World>().getScenes().menuSave();
				case MsgBox::Result::No:
					getModule<Window>().getWindow().close();
					return false;
					break;
				case MsgBox::Result::Cancel:
					break;
			}
		}


		//////////////////////
		switch(event.type)
        {
            case sf::Event::KeyPressed:
            {
                switch(event.key.code)
                {
                    case sf::Keyboard::P:
                        getModule<PrepScreen>().show();
                    break; 

                    default: break;
                }
            } 
            break;

            default: break;
        }
		//////////////////////


	}
	return true;
}

void Application::update()
{
	_imGuiStyler.update();

	[[maybe_unused]] auto deltaTime = _mainClock.restart().asFSeconds();
	getModule<GUI>().update(deltaTime);
	#ifdef GUI_TEST
	{
		getModule<GUITest>().update(deltaTime);
	}
	#endif
	getModule<Music>().update(deltaTime);


	/*
		Put other updates here
	*/
	
	#ifdef EDITOR
	{
		ImGui::SFML::Update(getModule<Window>().getWindow(), sf::seconds(deltaTime));
	}
	#endif

	getModule<World>().update(deltaTime);
	getModule<Input>().getManager().finishLogic();
	getModule<Cinematics>().update();
}

void Application::render()
{
	glEnable(GL_DEPTH_TEST);
	getModule<Window>().getWindow().clear(22.f, 20.f, 28.f, 255.f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	getModule<World>().render();
	
	getModule<Window>().pushGLStates();
	getModule<GUI>().render();
	#ifdef GUI_TEST
	{
		getModule<GUITest>().render();
	}
	#endif
	getModule<Window>().popGLStates();
	getModule<Cinematics>().render();

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
			if(input()) {
				update();
				render();
			}
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
