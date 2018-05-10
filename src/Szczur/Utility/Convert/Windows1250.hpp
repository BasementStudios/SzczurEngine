#pragma once

#include <string>

namespace rat
{

///
std::string mapWindows1250ToUtf8(const std::string& src);

///
std::string mapUtf8ToWindows1250(const std::string& src);

}
