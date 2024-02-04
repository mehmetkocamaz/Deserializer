#pragma once
#include <imgui.h>
#include <functional>
#include <string>
#include <format>
#include "Editor/InputValidator/CombineInfoValidator.h"

namespace ImGuiUtils
{
	static ImVec4 imDarkRed = ImVec4(0.8f, 0.0f, 0.0f, 1.0f);
	static ImVec4 imDarkGreen = ImVec4(0.0f, 0.5f, 0.0f, 1.0f);

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

	static void ValidatedInputScalar(E_InputType p_InputType, const char* pp_Label, ImGuiDataType p_DataType, uint32_t* pp_Data, const void* pp_Step, const void* pp_StepFast, const char* pp_Format, bool* pp_IsModified)
	{
		uint32_t v_TempData = *pp_Data;
		bool v_Possible = true;
		if (ImGui::InputScalar(std::format("##{}", pp_Label).c_str(), p_DataType, &v_TempData, pp_Step, pp_StepFast, pp_Format))
		{
			const ValidatorOutput& v_Output = CombineInfoValidator::Instance().Validate(p_InputType, v_TempData);
			if (v_Output.m_Result)
			{
				if (*pp_Data != v_TempData) {
					*pp_Data = v_TempData;
					*pp_IsModified = true;
				}
			}
			else
			{
				ImGui::SameLine();
				ImGui::TextColored(imDarkRed, std::format("! {} !", pp_Label).c_str());
				v_Possible = false;
			}
		}

		if (v_Possible)
		{
			ImGui::SameLine();
			ImGui::TextColored(imDarkGreen, pp_Label);
		}
	}
	
	static void ValidatedCombo(E_InputType p_InputType, const char* pp_Label, int32_t* pp_CurrentItem, const char* const pp_Items[], int32_t p_ItemsCount, int32_t p_HeightInItems, bool* pp_IsModified, bool* p_SelectedItems, int32_t p_SelectedCount) {

		int32_t v_TempData = *pp_CurrentItem;
		bool v_Possible = true;
		if (ImGui::Combo(std::format("##{}", pp_Label).c_str(), &v_TempData, pp_Items, p_ItemsCount)) {
			const ValidatorOutput& v_Output = CombineInfoValidator::Instance().Validate(p_InputType, v_TempData);
			if (v_Output.m_Result) {
				if (*pp_CurrentItem != v_TempData) {
					*pp_CurrentItem = v_TempData;
					*pp_IsModified = true;
					if (p_SelectedItems[*pp_CurrentItem] == true) {
						switch (p_InputType)
						{
						case E_InputType::TargetRequirementType: *pp_CurrentItem = (int32_t)Enum_Requirement::None; break;
						case E_InputType::SourceRequirementType: *pp_CurrentItem = (int32_t)Enum_Requirement::None; break;
						case E_InputType::SourceCostType: *pp_CurrentItem = (int32_t)Enum_Cost::None; break;
						}
					}
					else
						p_SelectedItems[*pp_CurrentItem] = true;
				}
			}
			else {
				*pp_CurrentItem = v_TempData;
				ImGui::SameLine();
				ImGui::TextColored(imDarkRed, std::format("! {} !", pp_Label).c_str());
				v_Possible = false;
			}
		}
		if (v_Possible) {
			ImGui::SameLine();
			ImGui::TextColored(imDarkGreen, pp_Label);
		}
	}
}