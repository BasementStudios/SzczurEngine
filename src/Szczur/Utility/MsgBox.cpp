#include "MsgBox.hpp"

#if defined(EDITOR) && defined(OS_WINDOWS)
#include <Windows.h>
#endif

namespace rat::MsgBox
{

int getIconId(Icon icon)
{
#if defined(EDITOR) && defined(OS_WINDOWS)
	switch (icon)
	{
		default:
		case Icon::Information:
			return MB_ICONINFORMATION;
		case Icon::Warning:
			return MB_ICONWARNING;
		case Icon::Error:
			return MB_ICONERROR;
		case Icon::Question:
			return MB_ICONQUESTION;
	}
#else
	return Icon::Information;
#endif
}

int getButtonId(Button button)
{
#if defined(EDITOR) && defined(OS_WINDOWS)
	switch (button)
	{
		default:
		case Button::Ok:
			return MB_OK;
		case Button::OkCancel:
			return MB_OKCANCEL;
		case Button::AbortRetryIgnore:
			return MB_ABORTRETRYIGNORE;
		case Button::YesNoCancel:
			return MB_YESNOCANCEL;
		case Button::YesNo:
			return MB_YESNO;
		case Button::RetryCancel:
			return MB_RETRYCANCEL;
	}
#else
	return Button::Ok;
#endif
}

Result show(const std::string& text, const std::string& title, Icon icon, Button button)
{
#if defined(EDITOR) && defined(OS_WINDOWS)
	int result = MessageBoxA(nullptr, text.c_str(), title.c_str(), getIconId(icon) | getButtonId(button));

	switch (result)
	{
		default:
		case IDOK:
			return Result::Ok;
		case IDCANCEL:
			return Result::Cancel;
		case IDABORT:
			return Result::Abort;
		case IDRETRY:
			return Result::Retry;
		case IDIGNORE:
			return Result::Ignore;
		case IDYES:
			return Result::Yes;
		case IDNO:
			return Result::No;
		case IDTRYAGAIN:
			return Result::TryAgain;
		case IDCONTINUE:
			return Result::Continue;
	}
#else
	return Result::Ok;
#endif
}

}
