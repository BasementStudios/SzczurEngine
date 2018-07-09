#pragma once

#include <string>

#include "Szczur/Config.hpp"

namespace rat::MsgBox
{

enum class Icon
{
	Information,
	Warning,
	Error,
	Question
};

enum class Button
{
	Ok,
	OkCancel,
	AbortRetryIgnore,
	YesNoCancel,
	YesNo,
	RetryCancel
};

enum class Result
{
	Ok,
	Cancel,
	Abort,
	Retry,
	Ignore,
	Yes,
	No,
	TryAgain,
	Continue
};

Result show(const std::string& text, const std::string& title = "", Icon icon = Icon::Information, Button button = Button::Ok);
Result show(void* hwnd, const std::string& text, const std::string& title = "", Icon icon = Icon::Information, Button button = Button::Ok);



}
