#include "ImGuiStyler.hpp"

#include <fstream>

#include <Szczur/Utility/Logger.hpp>

#include <nlohmann/json.hpp>
using Json = nlohmann::json;

#include <imgui.h>

#include "Szczur/Utility/Debug/NotoMono.ttf.bin"

#define SET_SHAPE_VAR(NAME, VALUE) if(auto& var = shape[#NAME]; !var.is_null()) style.NAME = VALUE;
#define SET_COLOR_VAR(NAME) if(auto& var = color[#NAME]; !var.is_null()) style.Colors[ ImGuiCol_ ## NAME ] = ImVec4(var[0],var[1],var[2],var[3]);

namespace rat {

	ImGuiStyler *ImGuiStyler::ptr = nullptr;

	///
	ImGuiStyler::ImGuiStyler() {
		if(ptr == nullptr) {
			ptr = this;
		}
		ImGui::CreateContext();	
	}

	///
	void ImGuiStyler::setupFont() {
		static ImWchar ranges[] = { 0x0020, 0x01FF, 0x0 };

		std::ifstream file(stylePath);
		if(file.good()) {

			Json config;
			file >> config;

			auto& font = config["Font"];
			if(!font.is_null()) {

				std::string fontPath = font["Path"].get<std::string>();
				float fontSize = font["Size"].get<float>();
				ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath.c_str(), fontSize, nullptr, ranges);
				LOG_INFO(fontPath, fontSize);
				return;
			}
		}
		ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(builtinFontData, builtinFontSize, 16.f, nullptr, ranges);

	}

	///
	ImGuiStyler& ImGuiStyler::get() {
		return *ptr;
	}

	///
	void ImGuiStyler::setPath(const std::string& path) {
		stylePath = path;
	}

	///
	void ImGuiStyler::setPathAndReload(const std::string& path) {
		setPath(path);
		reload();
	}

	///
	void ImGuiStyler::reload() {
		doUpdate = true;
	}

	///
	void ImGuiStyler::update() {
		if(!doUpdate) return;
		doUpdate = false;

		Json config;

		std::ifstream file(stylePath);
		if(!file.good()) return;

		file >> config;

    	ImGuiStyle& style = ImGui::GetStyle();

		// Shape
		auto& shape = config["Shape"];

		SET_SHAPE_VAR(Alpha,					var.get<float>());      	// Global alpha applies to everything in ImGui.
		SET_SHAPE_VAR(WindowPadding,			ImVec2(var[0], var[1]));	// Padding within a window.
		SET_SHAPE_VAR(WindowRounding,			var.get<float>());      	// Radius of window corners rounding. Set to 0.0f to have rectangular windows.
		SET_SHAPE_VAR(WindowBorderSize,			var.get<float>());      	// Thickness of border around windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
		SET_SHAPE_VAR(WindowMinSize,			ImVec2(var[0], var[1]));	// Minimum window size. This is a global setting. If you want to constraint individual windows, use SetNextWindowSizeConstraints().
		SET_SHAPE_VAR(WindowTitleAlign,			ImVec2(var[0], var[1]));	// Alignment for title bar text. Defaults to (0.0f,0.5f) for left-aligned,vertically centered.
		SET_SHAPE_VAR(ChildRounding,			var.get<float>());      	// Radius of child window corners rounding. Set to 0.0f to have rectangular windows.
		SET_SHAPE_VAR(ChildBorderSize,			var.get<float>());      	// Thickness of border around child windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
		SET_SHAPE_VAR(PopupRounding,			var.get<float>());      	// Radius of popup window corners rounding. (Note that tooltip windows use WindowRounding)
		SET_SHAPE_VAR(PopupBorderSize,			var.get<float>());      	// Thickness of border around popup/tooltip windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
		SET_SHAPE_VAR(FramePadding,				ImVec2(var[0], var[1]));	// Padding within a framed rectangle (used by most widgets).
		SET_SHAPE_VAR(FrameRounding,			var.get<float>());      	// Radius of frame corners rounding. Set to 0.0f to have rectangular frame (used by most widgets).
		SET_SHAPE_VAR(FrameBorderSize,			var.get<float>());      	// Thickness of border around frames. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
		SET_SHAPE_VAR(ItemSpacing,				ImVec2(var[0], var[1]));	// Horizontal and vertical spacing between widgets/lines.
		SET_SHAPE_VAR(ItemInnerSpacing,			ImVec2(var[0], var[1]));	// Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label).
		SET_SHAPE_VAR(TouchExtraPadding,		ImVec2(var[0], var[1]));	// Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
		SET_SHAPE_VAR(IndentSpacing,			var.get<float>());      	// Horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
		SET_SHAPE_VAR(ColumnsMinSpacing,		var.get<float>());      	// Minimum horizontal spacing between two columns.
		SET_SHAPE_VAR(ScrollbarSize,			var.get<float>());      	// Width of the vertical scrollbar, Height of the horizontal scrollbar.
		SET_SHAPE_VAR(ScrollbarRounding,		var.get<float>());      	// Radius of grab corners for scrollbar.
		SET_SHAPE_VAR(GrabMinSize,				var.get<float>());      	// Minimum width/height of a grab box for slider/scrollbar.
		SET_SHAPE_VAR(GrabRounding,				var.get<float>());      	// Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
		SET_SHAPE_VAR(ButtonTextAlign,			ImVec2(var[0], var[1]));	// Alignment of button text when button is larger than text. Defaults to (0.5f,0.5f) for horizontally+vertically centered.
		SET_SHAPE_VAR(DisplayWindowPadding,		ImVec2(var[0], var[1]));	// Window positions are clamped to be visible within the display area by at least this amount. Only covers regular windows.
		SET_SHAPE_VAR(DisplaySafeAreaPadding,	ImVec2(var[0], var[1]));	// If you cannot see the edges of your screen (e.g. on a TV) increase the safe area padding. Apply to popups/tooltips as well regular windows. NB: Prefer configuring your TV sets correctly!
		SET_SHAPE_VAR(MouseCursorScale,			var.get<float>());      	// Scale software rendered mouse cursor (when io.MouseDrawCursor is enabled). May be removed later.
		SET_SHAPE_VAR(AntiAliasedLines,			var.get<bool>());       	// Enable anti-aliasing on lines/borders. Disable if you are really tight on CPU/GPU.
		SET_SHAPE_VAR(AntiAliasedFill,			var.get<bool>());       	// Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.)
		SET_SHAPE_VAR(CurveTessellationTol,		var.get<float>());

		// Color
		auto& color = config["Color"];

		SET_COLOR_VAR(Text						);			
		SET_COLOR_VAR(TextDisabled				);			
		SET_COLOR_VAR(WindowBg					);	// Background of normal windows
		SET_COLOR_VAR(ChildBg					);	// Background of child windows
		SET_COLOR_VAR(PopupBg					);	// Background of popups, menus, tooltips windows
		SET_COLOR_VAR(Border					);			
		SET_COLOR_VAR(BorderShadow				);			
		SET_COLOR_VAR(FrameBg					);	// Background of checkbox, radio button, plot, slider, text input
		SET_COLOR_VAR(FrameBgHovered			);			
		SET_COLOR_VAR(FrameBgActive				);			
		SET_COLOR_VAR(TitleBg					);			
		SET_COLOR_VAR(TitleBgActive				);			
		SET_COLOR_VAR(TitleBgCollapsed			);			
		SET_COLOR_VAR(MenuBarBg					);			
		SET_COLOR_VAR(ScrollbarBg				);			
		SET_COLOR_VAR(ScrollbarGrab				);			
		SET_COLOR_VAR(ScrollbarGrabHovered		);			
		SET_COLOR_VAR(ScrollbarGrabActive		);			
		SET_COLOR_VAR(CheckMark					);			
		SET_COLOR_VAR(SliderGrab				);			
		SET_COLOR_VAR(SliderGrabActive			);			
		SET_COLOR_VAR(Button					);			
		SET_COLOR_VAR(ButtonHovered				);			
		SET_COLOR_VAR(ButtonActive				);			
		SET_COLOR_VAR(Header					);			
		SET_COLOR_VAR(HeaderHovered				);			
		SET_COLOR_VAR(HeaderActive				);			
		SET_COLOR_VAR(Separator					);			
		SET_COLOR_VAR(SeparatorHovered			);			
		SET_COLOR_VAR(SeparatorActive			);			
		SET_COLOR_VAR(ResizeGrip				);			
		SET_COLOR_VAR(ResizeGripHovered			);			
		SET_COLOR_VAR(ResizeGripActive			);			
		SET_COLOR_VAR(PlotLines					);			
		SET_COLOR_VAR(PlotLinesHovered			);			
		SET_COLOR_VAR(PlotHistogram				);			
		SET_COLOR_VAR(PlotHistogramHovered		);			
		SET_COLOR_VAR(TextSelectedBg			);			
		SET_COLOR_VAR(ModalWindowDarkening		);	// Darken/colorize entire screen behind a modal window, when one is active
		SET_COLOR_VAR(DragDropTarget			);			
		SET_COLOR_VAR(NavHighlight				);	// Gamepad/keyboard: current highlighted item
		SET_COLOR_VAR(NavWindowingHighlight		);	// Gamepad/keyboard: when holding NavMenu to focus/move/resize windows

	}

}

#undef SET_COLOR_VAR
#undef SET_SHAPE_VAR