#pragma once

#include <string>

namespace rat
{

///
std::string mapWindows1250toUtf8(const std::string& src);

///
std::string mapUtf8toWindows1250(const std::string& src);

}
