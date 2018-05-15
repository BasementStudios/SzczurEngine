#pragma once

#include "Szczur/Config.hpp"

#ifdef OS_WINDOWS
#	include <Windows.h>
#endif

namespace rat::file
{

time_t getLastModificationTime(const std::string& path);

time_t getCreationTime(const std::string& path);

}
