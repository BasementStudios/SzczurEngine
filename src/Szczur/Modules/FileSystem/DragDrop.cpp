#include "DragDrop.hpp"
#ifdef OS_WINDOWS
#   include <shellapi.h>
#endif
namespace rat
{

DragDrop* DragDrop::_this;

DragDrop::DragDrop()
{
	LOG_INFO("Initializing DragDrop module");
#ifdef EDITOR
	auto& window = getModule<Window>().getWindow();

	// accept dropping files
	DragAcceptFiles(window.getSystemHandle(), true);

	// hook wndproc
	_callback = SetWindowLongPtrW(window.getSystemHandle(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&DragDrop::WndProc));

	_this = this;
#endif
	LOG_INFO("Module DragDrop initialized");
}

DragDrop::~DragDrop()
{
	LOG_INFO("Module DragDrop destructed");
}

#ifdef EDITOR
LRESULT DragDrop::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DROPFILES:
		{
			std::vector<std::string> files;

			char fileName[1024] = { 0 };

			HDROP hDrop = reinterpret_cast<HDROP>(wParam);

			auto file = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);

			POINT pos;

			DragQueryPoint(hDrop, &pos);

			_this->_lastDropPos = { pos.x, pos.y };

			for (uint32_t i = 0; i < file; i++)
			{
				fileName[0] = '\0';

				if (DragQueryFile(hDrop, i, fileName, 1024))
				{
					_this->_droppedFiles.push_back(fileName);
				}
			}

			DragFinish(hDrop);
		} break;
	}

	return CallWindowProc(reinterpret_cast<WNDPROC>(_this->_callback), handle, message, wParam, lParam);
}
#endif

}
