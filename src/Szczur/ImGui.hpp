#pragma once

#if __has_include(<ImGui/imgui.h>) && __has_include(<ImGui/imgui-SFML.h>)
#   include <ImGui/imgui.h>
#   include <ImGui/imgui-SFML.h>
#elif __has_include(<imgui.h>) && __has_include(<imgui-SFML.h>)
#   include <imgui.h>
#   include <imgui-SFML.h>
#else
#	error No <imgui.h> and <imgui-SFML.h> headers are avaible
#endif

#define IF_EDITOR if constexpr(true)
