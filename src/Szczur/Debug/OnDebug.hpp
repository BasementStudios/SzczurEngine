#pragma once

#ifndef NDEBUG

#define rat_OnDebug(code) { code }

#else

#define rat_OnDebug(...)

#endif
