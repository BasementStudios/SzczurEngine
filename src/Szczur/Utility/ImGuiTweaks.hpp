#pragma once

#include <tuple>
#include <string>

#include <imgui.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace ImGui
{

inline std::tuple<float, glm::vec2, glm::vec3, int> clipboard;

enum Tweaks
{
	CopyPaste,
	Reset
};

template <Tweaks Tweak, typename T>
void AddTweak(T& v)
{
	int n = sizeof(T) / 4 - 1;

	if constexpr (Tweak == Tweaks::CopyPaste)
	{
		if (ImGui::Selectable("Copy##clipboard"))
		{
			std::get<T>(clipboard) = v;;

			if (std::is_same_v<T, float> || std::is_same_v<T, glm::vec2> || std::is_same_v<T, glm::vec3>)
			{
				std::string clipboardStr;

				for (int i = 0; i <= n; i++)
				{
					clipboardStr += std::to_string(*(reinterpret_cast<float*>(&v) + i));

					if (i != n)
						clipboardStr += ", ";
				}

				ImGui::SetClipboardText(clipboardStr.c_str());
			}
		}

		if (ImGui::Selectable("Paste##clipboard"))
		{
			v = std::get<T>(clipboard);
		}
	}
	else if constexpr (Tweak == Tweaks::Reset)
	{
		if (ImGui::Selectable("Reset##reset"))
		{
			memset(&v, 0, sizeof(T));
		}
	}
}

template <Tweaks... Ts, typename T>
void PopupContext(T& v, const std::string& label)
{
	if (ImGui::BeginPopupContextItem((label + "##popup").c_str()))
	{
		(AddTweak<Ts, T>(v), ...);

		ImGui::EndPopup();
	}
}

template <Tweaks... Ts>
bool DragFloat(const std::string& label, float& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.3f", float power = 1.0f)
{
	bool result = ImGui::DragFloat(label.c_str(), &v, v_speed, v_min, v_max, display_format, power);

	PopupContext<Ts...>(v, label);

	return result;
}

template <Tweaks... Ts>
bool DragVec2(const std::string& label, glm::vec2& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.3f", float power = 1.0f)
{
	bool result = ImGui::DragFloat2(label.c_str(), &v[0], v_speed, v_min, v_max, display_format, power);

	PopupContext<Ts...>(v, label);

	return result;
}

template <Tweaks... Ts>
bool DragVec3(const std::string& label, glm::vec3& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.3f", float power = 1.0f)
{
	bool result = ImGui::DragFloat3(label.c_str(), &v[0], v_speed, v_min, v_max, display_format, power);

	PopupContext<Ts...>(v, label);

	return result;
}

template <Tweaks... Ts>
bool InputFloat(const std::string& label, float& v, float step = 0.0f, float step_fast = 0.0f, int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0)
{
	bool result = ImGui::InputFloat(label.c_str(), &v, step, step_fast, decimal_precision, extra_flags);

	PopupContext<Ts...>(v, label);

	return result;
}

template <Tweaks... Ts>
bool InputVec2(const std::string& label, glm::vec2& v, int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0)
{
	bool result = ImGui::InputFloat2(label.c_str(), &v[0], decimal_precision, extra_flags);

	PopupContext<Ts...>(v, label);

	return result;
}

template <Tweaks... Ts>
bool InputVec3(const std::string& label, glm::vec3& v, int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0)
{
	bool result = ImGui::InputFloat3(label.c_str(), &v[0], decimal_precision, extra_flags);

	PopupContext<Ts...>(v, label);

	return result;
}

template <Tweaks... Ts>
bool InputInt(const std::string& label, int& v, int step = 1, int step_fast = 100, ImGuiInputTextFlags extra_flags = 0)
{
	bool result = ImGui::InputInt(label.c_str(), &v, step, step_fast, extra_flags);

	PopupContext<Ts...>(v, label);

	return result;
}



}
