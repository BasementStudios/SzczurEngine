#include "Window.hpp"

namespace rat
{

#ifdef EDITOR
Window* Window::_this;
#endif

Window::Window() :
	_window(sf::VideoMode(1600, 900), "Editor", sf::Style::Close)
{

#ifdef EDITOR
	// accept dropping files
	DragAcceptFiles(_window.getSystemHandle(), true);

	// hook wndproc
	_callback = SetWindowLongPtrW(_window.getSystemHandle(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::WndProc));
#endif

	_this = this;

	_window.setFramerateLimit(60);

	LOG_INFO(this, " : Module Window constructed");
}

Window::~Window()
{
	LOG_INFO(this, " : Module Window destructed");
}

void Window::clear(const sf::Color& color)
{
	_window.clear(color);
}

void Window::render()
{
	_window.display();
}

Window::Window_t& Window::getWindow()
{
	return _window;
}

const Window::Window_t& Window::getWindow() const
{
	return _window;
}

#ifdef EDITOR
LRESULT CALLBACK Window::WndProc(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam)
{
	switch (Message)
	{
		case WM_DROPFILES:
		{
			std::vector<std::string> files;

			char fileName[1024] = { 0 };

			HDROP hDrop = reinterpret_cast<HDROP>(WParam);

			auto file = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);

			POINT pos;

			DragQueryPoint(hDrop, &pos);

			_this->_lastDropPos = { pos.x, pos.y };

			for (int i = 0; i < file; i++)
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

	return CallWindowProc(reinterpret_cast<WNDPROC>(_this->_callback), Handle, Message, WParam, LParam);
}
#endif

}
