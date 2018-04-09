#pragma once

#include "Szczur/Utility/Modules/Module.hpp"

#include "Szczur/Modules/Window/Window.hpp"

#ifdef EDITOR
#include <Windows.h>
#endif

namespace rat
{

class DragDrop : public Module<Window>
{
public:
	DragDrop();

	DragDrop(const DragDrop&) = delete;
	DragDrop& operator = (const DragDrop&) = delete;

	DragDrop(DragDrop&&) = delete;
	DragDrop& operator = (DragDrop&&) = delete;

	~DragDrop();

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
	static DragDrop* _this;

#ifdef EDITOR
	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
#endif
};

}