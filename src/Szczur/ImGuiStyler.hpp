#pragma once

#include <string>

struct ImFont;
namespace rat {

class ImGuiStyler {
public:

	///
	ImGuiStyler();

	///
	void setupFont();

	///
	static ImGuiStyler& get();

	///
	void setPath(const std::string& path);

	///
	void setPathAndReload(const std::string& path);

	///
	void reload();

	///
	void update();

private:
	std::string stylePath = "Editor/Gui/style.json";
	static ImGuiStyler *ptr;
	bool doUpdate = false;
};
}