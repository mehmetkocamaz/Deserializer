#pragma once
#include <imgui.h>
#include <functional>
#include <string>

namespace ImGuiUtils
{
	static void ColoredButton(std::string text, const ImVec2& size, const ImVec4& color, const std::function<void()>& callback) {
		ImGui::PushStyleColor(ImGuiCol_Button, color);
		if (ImGui::Button(text.c_str(), size)) {
			callback();
		}
		ImGui::PopStyleColor();
	}

	static bool BorderedTreeNode(const char* label)
	{
		ImVec2 p_min = ImGui::GetCursorScreenPos();
		bool is_open = ImGui::TreeNode(label);
		if (is_open)
		{
			return true;
		}
		ImVec2 p_max = ImVec2(ImGui::GetItemRectMax().x, ImGui::GetItemRectMin().y + ImGui::GetTextLineHeightWithSpacing());
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->AddRect(p_min, p_max, IM_COL32(255, 0, 0, 255));  // red border
		return false;
	}

	static ImVec4 imDarkRed = ImVec4(0.8f, 0.0f, 0.0f, 1.0f);
	static ImVec4 imDarkGreen = ImVec4(0.0f, 0.5f, 0.0f, 1.0f);

}