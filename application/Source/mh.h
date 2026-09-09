#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "Colors.h"

#include <cstdint>
#include <algorithm>
#include <format>
#include <string>
#include <cstdio>
#include <vector>

namespace mh
{
	int lock = -1;

	void my_checkbox(const char* n, ImVec2 p, bool* v, ImDrawList* dl)
	{
		ImGuiIO& io = ImGui::GetIO();

		std::uint32_t border = Colors::Border;

		bool locked = lock != -1;
		bool is_hovering = locked ? false : ImGui::IsMouseHoveringRect(p, p + ImVec2(20, 20));

		if (is_hovering)
			border = Colors::Accent;

		if (*v)
		{
			//dl->AddRectFilled ( p, p + ImVec2 ( 20, 20 ), Bg );
			dl->AddRectFilled(p, p + ImVec2(20, 20), Colors::Accent, 4);

			dl->AddRect(p, p + ImVec2(20, 20), border, 4);
		}
		else
		{
			dl->AddRectFilled(p, p + ImVec2(20, 20), IM_COL32_WHITE, 4);
			dl->AddRect(p, p + ImVec2(20, 20), border, 4);
		}

		dl->AddText(io.Fonts->Fonts[0], 30, p + ImVec2(32, 10 - ImGui::CalcTextSize(n).y / 2), IM_COL32_BLACK, n);

		if (ImGui::IsKeyPressed(ImGuiKey_MouseLeft, false) and is_hovering)
			*v = not *v;
	}

	bool my_int_slider(const char* label, int* value, int min, int max, int id, ImVec2 pos_min, ImVec2 pos_max,
		ImDrawList* draw_list, std::string format)
	{
		ImGuiIO& io = ImGui::GetIO();
		bool locked = lock != id and lock != -1;

		ImRect sli = { {pos_min.x, (pos_max.y - pos_min.y) / 1.5f + pos_min.y}, {pos_max.x, pos_max.y} };
		//draw_list->AddRectFilled(pos_min, pos_max, IM_COL32(255, 0, 0, 100));

		draw_list->AddText(io.Fonts->Fonts[0], 30, pos_min, IM_COL32_BLACK, label);

		bool is_hovering = !locked && ImGui::IsMouseHoveringRect(sli.Min, sli.Max);
		bool is_sliding = lock == id;

		if (is_hovering) {
			int scroll_multiplier = ImGui::IsKeyDown(ImGuiKey_Space) ? 10 : 1;
			*value = static_cast<int>(*value - io.MouseWheel * scroll_multiplier);
		}

		*value = std::clamp(*value, min, max);
		float progress = float(*value - min) / (max - min);
		const int handle_pos = static_cast<int>(sli.Min.x + progress * (sli.Max.x - sli.Min.x));

		ImU32 border_color = (is_sliding || is_hovering) ? Colors::Accent : Colors::ActiveBorder;

		draw_list->AddRectFilled(sli.Min, sli.Max, Colors::Frame, 4);

		if (progress > 0.01)
		{
			draw_list->AddRectFilled(sli.Min, ImVec2(static_cast<float>(handle_pos), sli.Max.y), Colors::Accent, 4);
		}

		//draw_list->AddRect(pos_min, pos_max, border_color);

		char buffer[64];
		snprintf(buffer, sizeof(buffer), format.c_str(), *value);
		ImVec2 text_size = ImGui::CalcTextSize(buffer);
		ImVec2 text_pos = ImVec2(pos_max.x - text_size.x, pos_min.y);
		draw_list->AddText(io.Fonts->Fonts[0], 30, text_pos, IM_COL32_BLACK, buffer);
		draw_list->AddRect(sli.Min, sli.Max, IM_COL32_BLACK, 4);

		if (locked) return false;

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
			if (!is_sliding && is_hovering) {
				lock = id;
				return true;
			}
		}
		else {
			lock = -1;
		}

		if (lock == id) {
			ImVec2 mouse_pos = ImGui::GetMousePos();
			float normalized = (mouse_pos.x - sli.Min.x) / (sli.Max.x - sli.Min.x);
			normalized = std::clamp(normalized, 0.0f, 1.0f);
			*value = (int)(min + normalized * (max - min));
		}

		return false;
	}

	bool my_button(const char* n, ImRect rect, ImDrawList* dl, bool clickable)
	{
		ImGuiIO& io = ImGui::GetIO();
		bool result = false;
		std::uint32_t frame_color = IM_COL32_BLACK_TRANS;

		if (ImGui::IsMouseHoveringRect(rect.Min, rect.Max) and clickable)
		{
			frame_color = Colors::HoverFill;

			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left, false))
				result = true;
		}

		dl->AddRectFilled(rect.Min, rect.Max, frame_color, 4);
		dl->AddRect(rect.Min, rect.Max, Colors::ActiveBorder, 4);

		dl->AddText(io.Fonts->Fonts[0], 30, ImVec2(
			(rect.Max.x - rect.Min.x) / 2 + rect.Min.x,
			(rect.Max.y - rect.Min.y) / 2 + rect.Min.y
		) - ImGui::CalcTextSize(n) / 2, IM_COL32_BLACK, n);

		return result;
	}
}
