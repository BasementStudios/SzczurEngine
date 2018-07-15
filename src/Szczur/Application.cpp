#include "Application.hpp"

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
	initModule<Input>();
	initModule<Script>();
	initModule<GUI>();
	#ifdef GUI_TEST
	{
		initModule<GUITest>();
	}
	#endif

	LOG_INFO("Modules initialized");

	#ifdef EDITOR
	{
		LOG_INFO("Initializing ImGui");

		ImGui::CreateContext();
		static ImWchar ranges[] = { 0x0020, 0x01FF, 0x0 };
		ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(builtinFontData, builtinFontSize, 16.0f, nullptr, ranges);
		ImGui::SFML::Init(getModule<Window>().getWindow());

		ImGuiStyle& style = ImGui::GetStyle();

		style.FramePadding = { 5.0f, 5.0f };
		style.FrameRounding = 4.0f;
		style.ItemSpacing = { 12.0f, 8.0f };
		style.ItemInnerSpacing = { 8.0f, 6.0f };
		style.IndentSpacing = 25.0f;
		style.ScrollbarSize = 15.0f;

		style.Colors[ImGuiCol_Text] = { 0.80f, 0.80f, 0.83f, 1.00f };
		style.Colors[ImGuiCol_TextDisabled] = { 0.24f, 0.23f, 0.29f, 1.00f };
		style.Colors[ImGuiCol_WindowBg] = { 0.02f, 0.01f, 0.03f, 0.75f };
		style.Colors[ImGuiCol_ChildWindowBg] = { 0.07f, 0.07f, 0.09f, 0.00f };
		style.Colors[ImGuiCol_PopupBg] = { 0.07f, 0.07f, 0.09f, 1.00f };
		style.Colors[ImGuiCol_Border] = { 0.25f, 0.19f, 0.23f, 0.70f };
		style.Colors[ImGuiCol_BorderShadow] = { 0.92f, 0.91f, 0.88f, 0.00f };

		style.Colors[ImGuiCol_FrameBg] = { 0.17f, 0.08f, 0.13f, 1.00f };
		style.Colors[ImGuiCol_FrameBgHovered] = { 0.25f, 0.10f, 0.21f, 1.00f };
		style.Colors[ImGuiCol_FrameBgActive] = { 0.10f, 0.05f, 0.10f, 1.00f };

		style.Colors[ImGuiCol_TitleBg] = { 0.10f, 0.09f, 0.12f, 1.00f };
		style.Colors[ImGuiCol_TitleBgCollapsed] = { 1.00f, 0.98f, 0.95f, 0.75f };
		style.Colors[ImGuiCol_TitleBgActive] = { 0.07f, 0.07f, 0.09f, 1.00f };

		style.Colors[ImGuiCol_MenuBarBg] = { 0.10f, 0.09f, 0.12f, 1.00f };
		style.Colors[ImGuiCol_ScrollbarBg] = { 0.10f, 0.09f, 0.12f, 1.00f };
		style.Colors[ImGuiCol_ScrollbarGrab] = { 0.80f, 0.80f, 0.83f, 0.31f };
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = { 0.2f, 0.2f, 0.25f, 0.70f };
		style.Colors[ImGuiCol_ScrollbarGrabActive] = { 0.06f, 0.05f, 0.07f, 1.00f };
		style.Colors[ImGuiCol_CheckMark] = { 0.80f, 0.80f, 0.83f, 0.31f };

		style.Colors[ImGuiCol_SliderGrab] = { 0.25f, 0.10f, 0.21f, 1.00f };
		style.Colors[ImGuiCol_SliderGrabActive] = { 0.10f, 0.05f, 0.10f, 1.00f };

		style.Colors[ImGuiCol_Button] = { 0.17f, 0.08f, 0.13f, 1.00f };
		style.Colors[ImGuiCol_ButtonHovered] = { 0.25f, 0.10f, 0.21f, 1.00f };
		style.Colors[ImGuiCol_ButtonActive] = { 0.10f, 0.05f, 0.10f, 1.00f };

		style.Colors[ImGuiCol_Header] = { 0.13f, 0.08f, 0.17f, 1.00f };
		style.Colors[ImGuiCol_HeaderHovered] = { 0.21f, 0.10f, 0.25f, 1.00f };
		style.Colors[ImGuiCol_HeaderActive] = { 0.10f, 0.05f, 0.10f, 1.00f };

		style.Colors[ImGuiCol_Column] = { 0.17f, 0.08f, 0.13f, 1.00f };
		style.Colors[ImGuiCol_ColumnHovered] = { 0.25f, 0.10f, 0.21f, 1.00f };
		style.Colors[ImGuiCol_ColumnActive] = { 0.10f, 0.05f, 0.10f, 1.00f };

		style.Colors[ImGuiCol_ResizeGrip] = { 0.17f, 0.08f, 0.13f, 1.00f };
		style.Colors[ImGuiCol_ResizeGripHovered] = { 0.25f, 0.10f, 0.21f, 1.00f };
		style.Colors[ImGuiCol_ResizeGripActive] = { 0.10f, 0.05f, 0.10f, 1.00f };

		style.Colors[ImGuiCol_CloseButton] = { 0.17f, 0.08f, 0.13f, 1.00f };
		style.Colors[ImGuiCol_CloseButtonHovered] = { 0.25f, 0.10f, 0.21f, 1.00f };
		style.Colors[ImGuiCol_CloseButtonActive] = { 0.10f, 0.05f, 0.10f, 1.00f };

		style.Colors[ImGuiCol_PlotLines] = { 0.40f, 0.39f, 0.38f, 0.63f };
		style.Colors[ImGuiCol_PlotLinesHovered] = { 0.25f, 1.00f, 0.00f, 1.00f };
		style.Colors[ImGuiCol_PlotHistogram] = { 0.40f, 0.39f, 0.38f, 0.63f };
		style.Colors[ImGuiCol_PlotHistogramHovered] = { 0.25f, 1.00f, 0.00f, 1.00f };

		style.Colors[ImGuiCol_TextSelectedBg] = { 0.25f, 0.80f, 0.00f, 0.43f };
		style.Colors[ImGuiCol_ModalWindowDarkening] = { 0.08f, 0.07f, 0.10f, 0.73f };

		_isImGuiInitialized = true;

		LOG_INFO("ImGui initialized");
	}
	#endif
	
}

bool Application::input()
{
	sf::Event event;

	while (getModule<Window>().getWindow().pollEvent(event)) {
		getModule<Window>().processEvent(event);
		getModule<Input>().getManager().processEvent(event);
		getModule<GUI>().input(event);

		#ifdef GUI_TEST
		getModule<GUITest>().input(event);
		#endif

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
	getModule<GUI>().update(deltaTime);
	#ifdef GUI_TEST
	{
		getModule<GUITest>().update(deltaTime);
	}
	#endif
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
	getModule<GUI>().render();
	#ifdef GUI_TEST
	{
		getModule<GUITest>().render();
	}
	#endif
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
			if (input()) {
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
		if (_isImGuiInitialized)
		{
			ImGui::SFML::Shutdown();
		}
	}
	#endif

	LOG_INFO("Shutdowning application");

	return 0;
}

}
