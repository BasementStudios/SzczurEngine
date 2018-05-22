#pragma once

#if __has_include(<Lua/sol.hpp>)
#	include <Lua/sol.hpp>
#elif __has_include(<sol.hpp>)
#	include <sol.hpp>
#else
#	error No <sol.hpp> header is avaible
#endif
