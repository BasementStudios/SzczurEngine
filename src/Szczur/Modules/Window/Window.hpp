#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Debug.hpp"


#ifdef EDITOR
#include <Windows.h>
#include <string>
#endif

namespace rat
{

class Window : public Module<>
{
public:

	using Window_t = sf::RenderWindow;

	Window();

	~Window();

	Window(const Window&) = delete;

	Window& operator = (const Window&) = delete;

	Window(Window&&) = delete;

	Window& operator = (Window&&) = delete;

	void clear(const sf::Color& color = sf::Color::Transparent);

	void render();

	Window_t& getWindow();
	const Window_t& getWindow() const;

#ifdef EDITOR
public:
	// Check if are dropped files
	bool areFilesDropped() { return !_droppedFiles.empty(); }

	// gets list of dropped files
	// remember to clear buffer with clearDroppedFiles() after get!
	std::vector<std::string> getDroppedFiles() { return _droppedFiles; }

	// clear buffer of dropped files
	void clearDroppedFiles() { _droppedFiles.clear(); }

	// get last position of dropped files
	const sf::Vector2i& getLastDropPos() { return _lastDropPos; }

private:
	std::vector<std::string> _droppedFiles;

	sf::Vector2i _lastDropPos;

	uint64_t _callback;
	static Window* _this;

	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
#endif

private:

	Window_t _window;
};

}
