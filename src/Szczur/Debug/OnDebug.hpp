#pragma once

#ifndef NDEBUG
#	define ON_DEBUG(code) code
#else
#	define ON_DEBUG(...)
#endif
