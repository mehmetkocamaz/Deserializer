#pragma once
#include "imgui.h"
#include "Application/CombineInfoRoot.h"

namespace applicationUtils {
	static bool combineCriteriaTrailing = true;
	static bool sourceCriteriasTrailing = true;
	static bool targetRequirementInfoTrailing = true;

	void CombineInfoCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator) {
		bool isCombineInfoOpen = true;
		char combineInfoTabName[16];
		static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
		tab_bar_flags = ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);
		snprintf(combineInfoTabName, IM_ARRAYSIZE(combineInfoTabName), "%s %d", "Combine Info", combineInfoIterator + 1);

		if (ImGui::BeginTabItem(combineInfoTabName, &isCombineInfoOpen, ImGuiTabItemFlags_None))
		{
			ImGui::Text("%s", combineInfoTabName);
			ImGui::InputScalar("Target Item Id", ImGuiDataType_U32, &v_CombineInfo[combineInfoIterator].GetTargetItemIdRef(), NULL, NULL, "%u");
			if (ImGui::BeginTabBar("CombineCriteriaTabBar"), tab_bar_flags) {

				if (combineCriteriaTrailing)
					if (ImGui::TabItemButton("+ Combine Criteria", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
						CombineCriteria combineCriteria;
						v_CombineInfo[combineInfoIterator].PushCombineCriterias(combineCriteria);
					}

				CombineCriteriaCreator(v_CombineInfo, combineInfoIterator, tab_bar_flags);
				ImGui::EndTabBar;
			}
			ImGui::EndTabItem();
		}
	}
	void CombineCriteriaCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, ImGuiTabBarFlags& tab_bar_flags) {
		for (int32_t combineCriteriaIterator = 0; combineCriteriaIterator < v_CombineInfo[combineInfoIterator].GetCombineCriterias().size(); ) {
			bool isCombineCriteriaOpen = true;
			char combineCriteriaTabName[16];
			snprintf(combineCriteriaTabName, IM_ARRAYSIZE(combineCriteriaTabName), "%s %d", "Combine Criteria", combineCriteriaIterator + 1);

			if (ImGui::BeginTabItem(combineCriteriaTabName, &isCombineCriteriaOpen, ImGuiTabItemFlags_None)) {
				ImGui::Text("Combine Criteria %d", combineCriteriaIterator + 1);

				if (ImGui::BeginTabBar("CombineCriteriaExtensiveTabBar", tab_bar_flags))
				{
					if (sourceCriteriasTrailing)
						if (ImGui::TabItemButton("+ Source Criteria", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
						{
							SourceCriteria sourceCriteria;
							v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].PushSourceCriterias(sourceCriteria);
						}
					if (targetRequirementInfoTrailing)
						if (ImGui::TabItemButton("+ Target Requirement", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
							RequirementInfo targetRequirementInfo;
							v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].PushTargetRequirementInfo(targetRequirementInfo);
						}
					ImGui::EndTabItem();
				}
				ImGui::EndTabItem();
			}
		}
	}
	void SourceCriteriaCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator, ImGuiTabBarFlags& tab_bar_flags) {
		for (int32_t sourceCriteriaIterator = 0; sourceCriteriaIterator < v_CombineInfo[combineInfoIterator].GetCombineCriterias()[combineCriteriaIterator].GetSourceCriterias().size();) {

		}
	}
	void TargetRequirementInfoCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator, ImGuiTabBarFlags& tab_bar_flags) {
		for (int32_t targetRequirementInfoIterator = 0; targetRequirementInfoIterator < v_CombineInfo[combineInfoIterator].GetCombineCriterias()[combineCriteriaIterator].GetTargetRequirementInfo().size(); ) {
			RequirementInfo targetRequirementInfo;
			bool isTargetRequirementOpen = true;
			char targetRequirementTabName[30];
			snprintf(targetRequirementTabName, IM_ARRAYSIZE(targetRequirementTabName), "%s %d", "Target Requirement Info", targetRequirementInfoIterator + 1);

			if (ImGui::BeginTabItem(targetRequirementTabName, &isTargetRequirementOpen, ImGuiTabItemFlags_None))
			{
				Enum_Requirement requirementType;

				ImGui::Text("Requirement Type");
				if (ImGui::Button("New Requirement Info"))
				{

				}
				ImGui::EndTabBar();
			}
		}
	}

}