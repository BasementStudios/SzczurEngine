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
	initModule<Script>();
	initModule<Input>();
	initModule<DragonBones>();
	initModule<Camera>();
	initModule<World>();
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

	setupImGuiStyle();
}

bool Application::input()
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
			getModule<Window>().getWindow().close();
			return false;
		}
	}
	return true;
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
	}
	#endif

	getModule<DragonBones>().update(deltaTime);
	getModule<World>().update(deltaTime);
	getModule<Camera>().update();
	getModule<Input>().getManager().finishLogic();
}

void Application::render()
{	
	glEnable(GL_DEPTH_TEST);
	getModule<Window>().getWindow().clear(37.f, 37.f, 37.f, 255.f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	getModule<Window>().pushGLStates();
	getModule<GUI>().render();
	getModule<Window>().popGLStates();

	getModule<World>().render();

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
 

void Application::setupImGuiStyle() {
  
    ImGuiStyle& style = ImGui::GetStyle();
 
	// style.WindowPadding = ImVec2(15, 15);
	// style.WindowRounding = 5.0f;
	style.FramePadding = ImVec2(5, 5);
	style.FrameRounding = 4.0f;
	style.ItemSpacing = ImVec2(12, 8);
	style.ItemInnerSpacing = ImVec2(8, 6);
	style.IndentSpacing = 25.0f;
	style.ScrollbarSize = 15.0f;
	// style.ScrollbarRounding = 9.0f;
	// style.GrabMinSize = 5.0f;
	// style.GrabRounding = 3.0f;
 
	style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	// style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.02f, 0.01f, 0.03f, 0.75f);
	// style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 0.00f);	
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.25f, 0.19f, 0.23f, 0.70f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);

	// style.Colors[ImGuiCol_FrameBg] = ImVec4(0.19f, 0.10f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.17f, 0.08f, 0.13f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.10f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.10f, 0.05f, 0.10f, 1.00f);

	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);

	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.2f, 0.2f, 0.25f, 0.70f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	// style.Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);

	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.25f, 0.10f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.10f, 0.05f, 0.10f, 1.00f);

	style.Colors[ImGuiCol_Button] = ImVec4(0.17f, 0.08f, 0.13f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.10f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.10f, 0.05f, 0.10f, 1.00f);

	style.Colors[ImGuiCol_Header] = ImVec4(0.13f, 0.08f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.21f, 0.10f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.10f, 0.05f, 0.10f, 1.00f);

	style.Colors[ImGuiCol_Column] = ImVec4(0.17f, 0.08f, 0.13f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.25f, 0.10f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.10f, 0.05f, 0.10f, 1.00f);

	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.17f, 0.08f, 0.13f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.25f, 0.10f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.10f, 0.05f, 0.10f, 1.00f);

	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.17f, 0.08f, 0.13f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.25f, 0.10f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.10f, 0.05f, 0.10f, 1.00f);

	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);

	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 0.80f, 0.00f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.08f, 0.07f, 0.10f, 0.73f);
}
}