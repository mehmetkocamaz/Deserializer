#pragma once
#include "imgui.h"
#include "Application/CombineInfoRoot.h"
#include <format>
namespace applicationUtils {
	static bool combineCriteriaTrailing = true;
	static bool sourceCriteriasTrailing = true;
	static bool targetRequirementInfoTrailing = true;
	int32_t removeTargetIndex = 0;
	int32_t removeSourceRequirementIndex = 0;
	int32_t removeSourceCostIndex = 0;
	int32_t removeSourceProbabilityIndex = 0;
	//std::vector<bool> uniqueTargetReq{ false, false, false, false }; // 0 Enchantment, 1 Combine, 2 Refine, 3 Socket

	void CombineCriteriaCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator);
	void SourceCriteriaCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator);
	void TargetRequirementInfoCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator);
	void SourceRequirementInfoCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator, std::vector<SourceCriteria>& v_SourceCriterias, int32_t& sourceCriteriaIterator);
	void SourceCostInfoCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator, std::vector<SourceCriteria>& v_SourceCriterias, int32_t& sourceCriteriaIterator);
	void SourceProbabilityInfoCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator, std::vector<SourceCriteria>& v_SourceCriterias, int32_t& sourceCriteriaIterator);

	
	void CombineInfoCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator) {
		bool isCombineInfoOpen = true;
		char combineInfoTabName[16];
		static ImGuiTabBarFlags tab_bar_flags = /*ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable |*/ ImGuiTabBarFlags_TabListPopupButton | ImGuiTabBarFlags_FittingPolicyScroll;
		//tab_bar_flags |=  ImGuiTabBarFlags_FittingPolicyScroll;
		snprintf(combineInfoTabName, IM_ARRAYSIZE(combineInfoTabName), "%d %s", combineInfoIterator + 1, "Combine Info");

		if (v_CombineInfo[combineInfoIterator].GetCombineInfoStatusRef())
			if (ImGui::BeginTabItem(combineInfoTabName, &isCombineInfoOpen, ImGuiTabItemFlags_None))
			{
				ImGui::Text("%s", combineInfoTabName);
				ImGui::InputScalar("Target Item Id", ImGuiDataType_U32, &v_CombineInfo[combineInfoIterator].GetTargetItemIdRef(), NULL, NULL, "%u");
				if (ImGui::BeginTabBar("CombineCriteriaTabBar", tab_bar_flags)) {

					if (combineCriteriaTrailing)
						if (ImGui::TabItemButton("+ Combine Criteria", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
							CombineCriteria combineCriteria;
							combineCriteria.GetCombineCriteriaStatus() = true;
							v_CombineInfo[combineInfoIterator].PushCombineCriterias(combineCriteria);
						}

					CombineCriteriaCreator(v_CombineInfo, combineInfoIterator);
				
					ImGui::EndTabBar();
				}
				ImGui::EndTabItem();
			}

		if (!isCombineInfoOpen)
		{
			ImGui::OpenPopup(std::format("Delete Combine Info {0}", (combineInfoIterator + 1)).c_str());
		}
		if (ImGui::BeginPopupModal(std::format("Delete Combine Info {0}", (combineInfoIterator + 1)).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Are you sure you want to delete this combine info?");
			ImGui::Separator();
			if (ImGui::Button("OK"))
			{
				//v_CombineInfo.erase(v_CombineInfo.begin() + combineInfoIterator);
				v_CombineInfo[combineInfoIterator].GetCombineInfoStatusRef() = false;
				ImGui::CloseCurrentPopup(); 
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				combineInfoIterator++;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		++combineInfoIterator;
	}
	
	void CombineCriteriaCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator) {
		//static ImGuiTabBarFlags tab_bar_flags2 = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_TabListPopupButton | ImGuiTabBarFlags_FittingPolicyScroll;
		static ImGuiTabBarFlags tab_bar_flags2 = /*ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable |*/ ImGuiTabBarFlags_TabListPopupButton | ImGuiTabBarFlags_FittingPolicyScroll;

		for (int32_t combineCriteriaIterator = 0; combineCriteriaIterator < v_CombineInfo[combineInfoIterator].GetCombineCriterias().size(); ) {
			bool isCombineCriteriaOpen = true;
			char combineCriteriaTabName[30];
			snprintf(combineCriteriaTabName, IM_ARRAYSIZE(combineCriteriaTabName), "%d %s", combineCriteriaIterator + 1, "Combine Criteria");

			if(v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetCombineCriteriaStatus())
				if (ImGui::BeginTabItem(combineCriteriaTabName, &isCombineCriteriaOpen, ImGuiTabItemFlags_None)) {
					ImGui::Text("Combine Criteria %d", combineCriteriaIterator + 1);

					if (ImGui::BeginTabBar("CombineCriteriaExtensiveTabBar", tab_bar_flags2))
					{
						if (sourceCriteriasTrailing)
							if (ImGui::TabItemButton("+ Source Criteria", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
							{
								SourceCriteria sourceCriteria;
								sourceCriteria.GetSourceCriteriaStatusRef() = true;
								v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].PushSourceCriterias(sourceCriteria);
							}

						if (v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfo().size() == 0) {
							RequirementInfo requirementInfo;
							v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].PushTargetRequirementInfo(requirementInfo);
						}

						TargetRequirementInfoCreator(v_CombineInfo, combineInfoIterator, combineCriteriaIterator);
						SourceCriteriaCreator(v_CombineInfo, combineInfoIterator, combineCriteriaIterator);

						ImGui::EndTabBar();
					}
					ImGui::EndTabItem();
				}

			if (!isCombineCriteriaOpen)
			{
				ImGui::OpenPopup(std::format("Delete Combine Criteria {0}", (combineCriteriaIterator + 1)).c_str());
			}

			if (ImGui::BeginPopupModal(std::format("Delete Combine Criteria {0}", (combineCriteriaIterator + 1)).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Are you sure you want to delete this combine criteria?");
				ImGui::Separator();
				if (ImGui::Button("OK"))
				{
					//v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef().erase(v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef().begin() + combineCriteriaIterator);
					v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetCombineCriteriaStatus() = false;
					ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
					break;
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
				{
					combineCriteriaIterator++;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			combineCriteriaIterator++;
		}
	}
	void SourceCriteriaCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator) {
		
		//static ImGuiTabBarFlags tab_bar_flags3 = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
		static ImGuiTabBarFlags tab_bar_flags3 = /*ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable |*/ ImGuiTabBarFlags_TabListPopupButton | ImGuiTabBarFlags_FittingPolicyScroll;
		tab_bar_flags3 = ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);
		std::vector<SourceCriteria>& v_SourceCriterias = v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetSourceCriteriasRef();

		for (int32_t sourceCriteriaIterator = 0; sourceCriteriaIterator < v_SourceCriterias.size();) 
		{
			bool isSourceCriteriaOpen = true;
			char sourceCriteriaName[30];

			snprintf(sourceCriteriaName, IM_ARRAYSIZE(sourceCriteriaName), "%d %s", sourceCriteriaIterator + 1, "Source Criteria");
			
			if(v_SourceCriterias[sourceCriteriaIterator].GetSourceCriteriaStatusRef())
				if (ImGui::BeginTabItem(sourceCriteriaName, &isSourceCriteriaOpen, ImGuiTabItemFlags_None))
				{
					ImGui::NewLine();
				
					ImGui::InputScalar("Source Item Id", ImGuiDataType_U32, &v_SourceCriterias[sourceCriteriaIterator].GetSourceItemIdRef(), NULL, NULL, "%u");
					ImGui::NewLine();
					if (ImGui::BeginTabBar("SourceCriteriaExtensiveTabBar",tab_bar_flags3))
					{
						SourceRequirementInfoCreator(v_CombineInfo, combineInfoIterator, combineCriteriaIterator, v_SourceCriterias, sourceCriteriaIterator);
						SourceCostInfoCreator(v_CombineInfo, combineInfoIterator, combineCriteriaIterator, v_SourceCriterias, sourceCriteriaIterator);
						// Probability Info will have default 3 objects
						if (v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfos().size() == 0) {
							for (int32_t probabilityDefaultIterator = 0; probabilityDefaultIterator < 3; probabilityDefaultIterator++)
							{
								ProbabilityInfo probabilityInfo;
								probabilityInfo.m_ProbabilityType = static_cast<Enum_Probability>(probabilityDefaultIterator);
								v_SourceCriterias[sourceCriteriaIterator].SetProbabilityInfo(probabilityInfo);
							}
						}
						SourceProbabilityInfoCreator(v_CombineInfo, combineInfoIterator, combineCriteriaIterator, v_SourceCriterias, sourceCriteriaIterator);
						ImGui::EndTabBar();
					}
					ImGui::EndTabItem();
				}

			if (!isSourceCriteriaOpen)
			{
				ImGui::OpenPopup(std::format("Delete Source Criteria {0}", (sourceCriteriaIterator + 1)).c_str());
			}

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal(std::format("Delete Source Criteria {0}", (sourceCriteriaIterator + 1)).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Are you sure you want to delete this source criteria?");
				ImGui::Separator();
				if (ImGui::Button("OK"))
				{
					//v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetSourceCriteriasRef().erase(v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetSourceCriteriasRef().begin() + sourceCriteriaIterator);
					v_SourceCriterias[sourceCriteriaIterator].GetSourceCriteriaStatusRef() = false;
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
				{
					sourceCriteriaIterator++;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			sourceCriteriaIterator++;
		}
	}
	void TargetRequirementInfoCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator) {
		bool isTargetReqOpen = true;
		char removeTargetRequirementName[16];
		bool uniqueTargetReq[4] = { false, false, false, false }; // 0 Enchantment, 1 Combine, 2 Refine, 3 Socket
		if (ImGui::BeginTabItem("TargetRequirements", NULL, ImGuiTabItemFlags_Leading))
		{
			ImGui::NewLine();
			if (ImGui::Button("Add New Target Requirement"))
			{
				RequirementInfo targetRequirementInfo;
				if (v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfoRef().size() < 4)
					v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].PushTargetRequirementInfo(targetRequirementInfo);
			}

			char comboName[30];
			const char* buffer[4] = { "Enchanment" , "Combine" , "Refine", "Socket" };

			for (int32_t targetRequirementInfoIterator = 0; targetRequirementInfoIterator < v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfoRef().size();)
			{
				ImGui::NewLine();
				snprintf(comboName, IM_ARRAYSIZE(comboName), "%d %s", targetRequirementInfoIterator + 1, "Requirement Type");
				snprintf(removeTargetRequirementName, IM_ARRAYSIZE(removeTargetRequirementName), "%s %d", "Remove Index", targetRequirementInfoIterator + 1);
				if (ImGui::Combo(comboName, (int32_t*)&v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfoRef()[targetRequirementInfoIterator].m_RequirementType, buffer, IM_ARRAYSIZE(buffer))) {}
				char requirementValueName[30];
				snprintf(requirementValueName, IM_ARRAYSIZE(requirementValueName), "%d %s", targetRequirementInfoIterator + 1, "Requirement Value");
				ImGui::InputScalar(requirementValueName, ImGuiDataType_U32, &v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfoRef()[targetRequirementInfoIterator].m_RequirementValue, NULL, NULL, "%u");

				if (ImGui::Button(removeTargetRequirementName))
				{
					v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfoRef().erase(v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfoRef().begin() + targetRequirementInfoIterator);
				}

				if (!isTargetReqOpen)
					v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfoRef().erase(v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfoRef().begin() + targetRequirementInfoIterator);
				else
					targetRequirementInfoIterator++;

			}
			ImGui::EndTabItem();
		}
	}

	void SourceRequirementInfoCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator, std::vector<SourceCriteria>& v_SourceCriterias, int32_t& sourceCriteriaIterator) 
	{
		bool isSourceRequirementOpen = true;
		if (ImGui::BeginTabItem("Source Requirements", NULL, ImGuiTabItemFlags_None))
		{
			ImGui::NewLine();
			char comboName[30];
			char removeSourceRequirementName[16];
			const char* buffer[4] = { "Enchanment" , "Combine" , "Refine", "Socket" };

			if (ImGui::Button("Add New Requirement Info"))
			{
				if (v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfos().size() < 4)
				{
					RequirementInfo requirementInfo;
					v_SourceCriterias[sourceCriteriaIterator].SetSourceRequirementInfo(requirementInfo);
				}
			}

			for (int32_t sourceRequiremenIterator = 0; sourceRequiremenIterator < v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfos().size();)
			{
				ImGui::Text("\n");
				snprintf(comboName, IM_ARRAYSIZE(comboName), "%d %s", sourceRequiremenIterator + 1, "Requirement Type");
				snprintf(removeSourceRequirementName, IM_ARRAYSIZE(removeSourceRequirementName), "%s %d", "Remove Index", sourceRequiremenIterator + 1);
				if (ImGui::Combo(comboName, (int32_t*)&v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetSourceCriteriasRef()[sourceCriteriaIterator].GetSourceRequirementInfosRef()[sourceRequiremenIterator].m_RequirementType, buffer, IM_ARRAYSIZE(buffer))) {}
				char requirementValueName[30];
				snprintf(requirementValueName, IM_ARRAYSIZE(requirementValueName), "%d %s", sourceRequiremenIterator + 1, "Requirement Value");
				ImGui::InputScalar(requirementValueName, ImGuiDataType_U32, &v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef()[sourceRequiremenIterator].m_RequirementValue, NULL, NULL, "%u");

				if (ImGui::Button(removeSourceRequirementName))
				{
					v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef().erase(v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef().begin() + sourceRequiremenIterator);
				}

				if (!isSourceRequirementOpen)
					v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef().erase(v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef().begin() + sourceRequiremenIterator);
				else
					sourceRequiremenIterator++;
			}

			ImGui::EndTabItem();
		}
	}

	void SourceCostInfoCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator, std::vector<SourceCriteria>& v_SourceCriterias, int32_t& sourceCriteriaIterator)
	{
		bool isSourceCostInfoOpen = true;
		char removeSourceCostName[16];
		if (ImGui::BeginTabItem("Cost Infos", NULL, ImGuiTabItemFlags_None))
		{
			ImGui::NewLine();

			if (ImGui::Button("Add New Cost Info"))
			{
				if (v_SourceCriterias[sourceCriteriaIterator].GetCostInfos().size() < 4)
				{
					CostInfo costInfo;
					v_SourceCriterias[sourceCriteriaIterator].SetCostInfo(costInfo);
				}
			}

			ImGui::NewLine();
			for (int32_t costInfoIterator = 0; costInfoIterator < v_SourceCriterias[sourceCriteriaIterator].GetCostInfos().size();)
			{
				char comboName[30];
				const char* buffer[4] = { "Silver" , "Billion" , "ContributionPoint" , "BloodPoint" };
				//snprintf(costInfoName, IM_ARRAYSIZE(costInfoName), "%d %s", costInfoIterator + 1, "Cost Info");
				snprintf(comboName, IM_ARRAYSIZE(comboName), "%d %s", costInfoIterator + 1, "Cost Type");
				if (ImGui::Combo(comboName, (int32_t*)&v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetSourceCriteriasRef()[sourceCriteriaIterator].GetCostInfosRef()[costInfoIterator].m_CostType, buffer, IM_ARRAYSIZE(buffer))) {}
				char costValueName[30];
				snprintf(costValueName, IM_ARRAYSIZE(costValueName), "%d %s", costInfoIterator + 1, "Cost Value");
				snprintf(removeSourceCostName, IM_ARRAYSIZE(removeSourceCostName), "%s %d", "Remove Index", costInfoIterator + 1);
				ImGui::InputScalar(costValueName, ImGuiDataType_U32, &v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef()[costInfoIterator].m_CostValue, NULL, NULL, "%u");
				if (ImGui::Button(removeSourceCostName))
				{
					v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef().erase(v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef().begin() + costInfoIterator);
				}
				ImGui::NewLine();

				if (!isSourceCostInfoOpen)
					v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef().erase(v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef().begin() + costInfoIterator);
				else
					costInfoIterator++;
			}
			ImGui::EndTabItem();
		}
	}

	void SourceProbabilityInfoCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator, std::vector<SourceCriteria>& v_SourceCriterias, int32_t& sourceCriteriaIterator)
	{
		bool isProbabiltyOpen = true;
		if (ImGui::BeginTabItem("Probability Infos", NULL, ImGuiTabItemFlags_None))
		{
			ImGui::NewLine();

			for (int32_t probabiltyInfoIterator = 0; probabiltyInfoIterator < v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfos().size();)
			{
				const char* buffer[3] = { "Success" , "Fail" , "Break" };
				ImGui::BulletText(buffer[probabiltyInfoIterator]);
				char costValueName[30];
				snprintf(costValueName, IM_ARRAYSIZE(costValueName), "%s %d", "Probability Value", probabiltyInfoIterator + 1);
				ImGui::InputScalar(costValueName, ImGuiDataType_U32, &v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfosRef()[probabiltyInfoIterator].m_ProbabilityValue, NULL, NULL, "%u");
				double overallProbabiltyValues = 0.0f;
				for (int32_t probabilityRatio = 0; probabilityRatio < v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfos().size(); probabilityRatio++)
				{
					overallProbabiltyValues += v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfosRef()[probabilityRatio].m_ProbabilityValue;
				}
				
				if (overallProbabiltyValues != 0) {
					ImGui::Text("%f", (v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfosRef()[probabiltyInfoIterator].m_ProbabilityValue * (100.0f / overallProbabiltyValues)));
				}
				else
				{
					ImGui::Text("%f", 0.0f);
				}


				ImGui::NewLine();

				if (!isProbabiltyOpen)
					v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfosRef().erase(v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfosRef().begin() + probabiltyInfoIterator);
				else
					probabiltyInfoIterator++;
			}
			ImGui::EndTabItem();
		}
	}
}