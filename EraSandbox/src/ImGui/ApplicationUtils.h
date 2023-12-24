#pragma once
#include "imgui.h"
#include "Application/CombineInfoRoot.h"
#include <format>
#include "SerializeManager/SerializeManager.h"
#include <Windows.h>
#include <chrono>
#include <thread>

namespace applicationUtils {
	static int32_t saveProcess = 0;
	static int32_t combineInfoDisplayIterator = 0;
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
	void ShowcaseInit(std::vector<CombineInfo>& v_CombineInfos, int32_t openedCombineInfoIndex);
	void SaveFileWindowsDialog(const char* filter, char* outPath, int bufferSize);
	void SaveOperation(std::vector<CombineInfo>& v_CombineInfos, bool& saveThreadRunning);
	
	void CombineInfoCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator) {
		bool isCombineInfoOpen = true;
		char combineInfoTabName[16];
		static ImGuiTabBarFlags tab_bar_flags = /*ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable |*/ ImGuiTabBarFlags_TabListPopupButton | ImGuiTabBarFlags_FittingPolicyScroll;
		//tab_bar_flags |=  ImGuiTabBarFlags_FittingPolicyScroll;
		snprintf(combineInfoTabName, IM_ARRAYSIZE(combineInfoTabName), "%d %s", combineInfoIterator + 1, "Combine Info");

		if (v_CombineInfo[combineInfoIterator].GetCombineInfoStatusRef())
			if (ImGui::BeginTabItem(combineInfoTabName, &isCombineInfoOpen, ImGuiTabItemFlags_None))
			{
				if(ImGui::IsItemFocused())
				{
					combineInfoDisplayIterator = combineInfoIterator;
				}
				if (combineInfoDisplayIterator >= 0)
				{
					ShowcaseInit(v_CombineInfo, combineInfoIterator);
				}

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
						SourceCostInfoCreator(v_CombineInfo, combineInfoIterator, combineCriteriaIterator, v_SourceCriterias, sourceCriteriaIterator);
						SourceRequirementInfoCreator(v_CombineInfo, combineInfoIterator, combineCriteriaIterator, v_SourceCriterias, sourceCriteriaIterator);
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
			const char* buffer[5] = { "Enchanment" , "Combine" , "Refine", "Socket" , "None" };

			for (int32_t targetRequirementInfoIterator = 0; targetRequirementInfoIterator < v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfoRef().size();)
			{
				ImGui::NewLine();
				snprintf(comboName, IM_ARRAYSIZE(comboName), "%d %s", targetRequirementInfoIterator + 1, "Requirement Type");
				snprintf(removeTargetRequirementName, IM_ARRAYSIZE(removeTargetRequirementName), "%s %d", "Remove Index", targetRequirementInfoIterator + 1);
				if (ImGui::Combo(comboName, (int32_t*)&v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfoRef()[targetRequirementInfoIterator].m_RequirementType, buffer, IM_ARRAYSIZE(buffer))) 
				{
					for(int32_t comboIterator = 0; comboIterator < v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfoRef().size(); comboIterator++)
					{
						RequirementInfo& v_RequirementInfo = v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfoRef()[comboIterator];
						if(v_RequirementInfo.m_RequirementType == v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfoRef()[targetRequirementInfoIterator].m_RequirementType && (comboIterator != targetRequirementInfoIterator))
						{
							if (v_RequirementInfo.m_RequirementType != Enum_Requirement::None)
								v_RequirementInfo.m_RequirementType = Enum_Requirement::None;
						}
					}
				}
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
			const char* buffer[5] = { "Enchanment" , "Combine" , "Refine", "Socket", "None" };

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
				if (ImGui::Combo(comboName, (int32_t*)&v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetSourceCriteriasRef()[sourceCriteriaIterator].GetSourceRequirementInfosRef()[sourceRequiremenIterator].m_RequirementType, buffer, IM_ARRAYSIZE(buffer))) 
				{
					for (int32_t comboIterator = 0; comboIterator < v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef().size(); comboIterator++)
					{
						//RequirementInfo& v_RequirementInfo = v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfoRef()[comboIterator];
						RequirementInfo& v_SourceRequirementInfo = v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef()[comboIterator];
						if (v_SourceRequirementInfo.m_RequirementType == v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef()[sourceRequiremenIterator].m_RequirementType && (comboIterator != sourceRequiremenIterator))
						{
							if (v_SourceRequirementInfo.m_RequirementType != Enum_Requirement::None)
								v_SourceRequirementInfo.m_RequirementType = Enum_Requirement::None;
						}
					}
				}
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
				const char* buffer[5] = { "Silver" , "Billion" , "ContributionPoint" , "BloodPoint" , "None" };
				//snprintf(costInfoName, IM_ARRAYSIZE(costInfoName), "%d %s", costInfoIterator + 1, "Cost Info");
				snprintf(comboName, IM_ARRAYSIZE(comboName), "%d %s", costInfoIterator + 1, "Cost Type");
				if (ImGui::Combo(comboName, (int32_t*)&v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetSourceCriteriasRef()[sourceCriteriaIterator].GetCostInfosRef()[costInfoIterator].m_CostType, buffer, IM_ARRAYSIZE(buffer))) 
				{
					for(int32_t comboIterator = 0; comboIterator < v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef().size();comboIterator++) 
					{
						CostInfo& v_CostInfos = v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef()[comboIterator];
						if (v_CostInfos.m_CostType == v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef()[costInfoIterator].m_CostType && (comboIterator != costInfoIterator))
						{
							if (v_CostInfos.m_CostType != Enum_Cost::None)
								v_CostInfos.m_CostType = Enum_Cost::None;
						}
					}
				}
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

	void ShowcaseInit(std::vector<CombineInfo>& v_CombineInfos, int32_t openedCombineInfoIndex)
	{
		// Yeni bir combine criteria ekledikten sonra eklenen ilk source criteria içinde patlýyor
		ImGui::Begin("Showcase");
		ImGuiTableFlags table_flags = (ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable);
		if (ImGui::BeginTable("CombineInfoTable", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable))
		{
			char buffer[30];
			snprintf(buffer, IM_ARRAYSIZE(buffer), "%s %d", "Combine Info", (openedCombineInfoIndex + 1));
			ImGui::TableSetupColumn(buffer);
			ImGui::TableHeadersRow();

			ImGui::TableNextColumn();
			ImGui::Text("Target Item Id : %d", v_CombineInfos[openedCombineInfoIndex].GetTargetItemId());

			for (int32_t criteriaIterator = 0; criteriaIterator < v_CombineInfos[openedCombineInfoIndex].GetCombineCriteriasRef().size(); criteriaIterator++)
			{
				char buffer[30];
				snprintf(buffer, IM_ARRAYSIZE(buffer), "%s %d", "Combine Criteria", (criteriaIterator + 1));

				if(v_CombineInfos[openedCombineInfoIndex].GetCombineCriteriasRef().size() != 0)
					if (ImGui::BeginTable("CombineCriteriaTable", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable))
					{
						ImGui::TableSetupColumn(buffer);
						ImGui::TableHeadersRow();

						ImGui::TableNextColumn();
						ImGui::Text("Target Requirement Size : %d", v_CombineInfos[openedCombineInfoIndex].GetCombineCriteriasRef()[criteriaIterator].GetTargetRequirementInfoRef().size());
						ImGui::Text("Source Criteria Size : %d", v_CombineInfos[openedCombineInfoIndex].GetCombineCriteriasRef()[criteriaIterator].GetSourceCriteriasRef().size());

						if(v_CombineInfos[openedCombineInfoIndex].GetCombineCriteriasRef()[criteriaIterator].GetTargetRequirementInfoRef().size() != 0)
							if (ImGui::BeginTable("TargetRequirementTable", 2, table_flags))
							{
								ImGui::TableSetupColumn("Target Requirement Type");
								ImGui::TableSetupColumn("Target Requirement Value");
								ImGui::TableHeadersRow();
								for (int32_t targetRequirementIterator = 0; targetRequirementIterator < v_CombineInfos[openedCombineInfoIndex].GetCombineCriteriasRef()[criteriaIterator].GetTargetRequirementInfoRef().size(); targetRequirementIterator++)
								{
									ImGui::TableNextRow(ImGuiTableFlags_None);
									ImGui::TableNextColumn();
									switch (v_CombineInfos[openedCombineInfoIndex].GetCombineCriteriasRef()[criteriaIterator].GetTargetRequirementInfoRef()[targetRequirementIterator].m_RequirementType)
									{
									case Enum_Requirement::None:
										ImGui::Text("None");
										break;
									case Enum_Requirement::Enchanment:
										ImGui::Text("Enchanment");
										break;
									case Enum_Requirement::Combine:
										ImGui::Text("Combine");
										break;
									case Enum_Requirement::Refine:
										ImGui::Text("Refine");
										break;
									case Enum_Requirement::Socket:
										ImGui::Text("Socket");
										break;
									default:
										ImGui::Text("Invalid Choise");
										break;
									}
									ImGui::TableNextColumn();
									ImGui::Text("%d", v_CombineInfos[openedCombineInfoIndex].GetCombineCriteriasRef()[criteriaIterator].GetTargetRequirementInfoRef()[targetRequirementIterator].m_RequirementValue);
								}
								ImGui::EndTable();
							}

						std::vector<SourceCriteria>& v_SourceCriterias = v_CombineInfos[openedCombineInfoIndex].GetCombineCriteriasRef()[criteriaIterator].GetSourceCriteriasRef();

						for (int32_t sourceCriteriaIterator = 0; sourceCriteriaIterator < v_SourceCriterias.size(); sourceCriteriaIterator++)
						{
							char buffer[30];
							snprintf(buffer, IM_ARRAYSIZE(buffer), "%s %d", "Source Criteria", (sourceCriteriaIterator + 1));
							if(v_SourceCriterias.size() != 0)
								if (ImGui::BeginTable(buffer, 1, table_flags))
								{
									ImGui::TableSetupColumn(buffer);
									ImGui::TableHeadersRow();
									ImGui::TableNextColumn();
									ImGui::Text("Source Item Id : %d", v_SourceCriterias[sourceCriteriaIterator].GetSourceItemId());
									if (v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef().size() != 0)
										if (ImGui::BeginTable("SourceRequirementTable", 2, table_flags))
										{
											ImGui::TableSetupColumn("Source Requirement Type");
											ImGui::TableSetupColumn("Source Requirement Value");
											ImGui::TableHeadersRow();

											for (int32_t sourceRequirementIterator = 0; sourceRequirementIterator < v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef().size(); sourceRequirementIterator++)
											{
												ImGui::TableNextRow(ImGuiTableFlags_None);
												ImGui::TableNextColumn();
												switch (v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef()[sourceRequirementIterator].m_RequirementType)
												{
												case Enum_Requirement::None:
													ImGui::Text("None");
													break;
												case Enum_Requirement::Enchanment:
													ImGui::Text("Enchanment");
													break;
												case Enum_Requirement::Combine:
													ImGui::Text("Combine");
													break;
												case Enum_Requirement::Refine:
													ImGui::Text("Refine");
													break;
												case Enum_Requirement::Socket:
													ImGui::Text("Socket");
													break;
												default:
													ImGui::Text("Invalid Choise");
													break;
												}
												ImGui::TableNextColumn();
												ImGui::Text("%d", v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef()[sourceRequirementIterator].m_RequirementValue);
											}
											ImGui::EndTable();
										}

									if (v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef().size() != 0)
										if (ImGui::BeginTable("SourceCostTable", 2, table_flags))
										{
											ImGui::TableSetupColumn("Source Cost Type");
											ImGui::TableSetupColumn("Source Cost Value");
											ImGui::TableHeadersRow();
											for (int32_t sourceCostIterator = 0; sourceCostIterator < v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef().size(); sourceCostIterator++)
											{
												ImGui::TableNextRow(ImGuiTableFlags_None);
												ImGui::TableNextColumn();
												switch (v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef()[sourceCostIterator].m_CostType)
												{
												case Enum_Cost::None:
													ImGui::Text("None");
													break;
												case Enum_Cost::Billion:
													ImGui::Text("Billion");
													break;
												case Enum_Cost::BloodPoint:
													ImGui::Text("BloodPoint");
													break;
												case Enum_Cost::ContributionPoint:
													ImGui::Text("ContributionPoint");
													break;
												case Enum_Cost::Silver:
													ImGui::Text("Silver");
													break;
												default:
													ImGui::Text("Invalid Choise");
													break;
												}
												ImGui::TableNextColumn();
												ImGui::Text("%d", v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef()[sourceCostIterator].m_CostValue);
											}
											ImGui::EndTable();
										}

									if (v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfosRef().size() != 0)
										if (ImGui::BeginTable("SourceProbabilityInfo", 2, table_flags))
										{
											ImGui::TableSetupColumn("Source Probability Type");
											ImGui::TableSetupColumn("Source Probability Value");
											ImGui::TableHeadersRow();
											for (int32_t sourceProbabilityIterator = 0; sourceProbabilityIterator < v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfosRef().size(); sourceProbabilityIterator++)
											{
												ImGui::TableNextRow(ImGuiTableFlags_None);
												ImGui::TableNextColumn();
												switch (v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfosRef()[sourceProbabilityIterator].m_ProbabilityType)
												{
												case Enum_Probability::Success:
													ImGui::Text("Success");
													break;
												case Enum_Probability::Fail:
													ImGui::Text("Fail");
													break;
												case Enum_Probability::Break:
													ImGui::Text("Break");
													break;
												default:
													ImGui::Text("Invalid Choise");
													break;
												}
												ImGui::TableNextColumn();
												ImGui::Text("%d", v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfosRef()[sourceProbabilityIterator].m_ProbabilityValue);
											}
											ImGui::EndTable();
										}
									ImGui::EndTable();
								}
						}
						ImGui::EndTable();
					}
			}
			ImGui::EndTable();
		}
		ImGui::End();
	}

	void SaveFileWindowsDialog(const char* filter, char* outPath, int bufferSize)
	{
		OPENFILENAME openFileName;
		ZeroMemory(&openFileName, sizeof(openFileName));
		openFileName.lStructSize = sizeof(openFileName);
		openFileName.lpstrFilter = filter;
		openFileName.lpstrFile = outPath;
		openFileName.nMaxFile = bufferSize;
		openFileName.lpstrTitle = "Save to";
		openFileName.Flags = OFN_OVERWRITEPROMPT;

		if(GetSaveFileName(&openFileName) == TRUE)
		{
		
		}
	}

	void SaveOperation(std::vector<CombineInfo>& v_CombineInfos, bool& saveThreadRunning)
	{
		SerializeSpec spec;
		spec.m_ContentType = Enum_SerizalizeContentType::BINARY;
		spec.m_CombineInfos = &v_CombineInfos;
		SerializerManager serializerManager(spec);
		std::this_thread::sleep_for(std::chrono::seconds(5));
		if (serializerManager.CheckForNone() == Enum_SerializationStatus::TYPE_NONE)
		{
			saveProcess = 1;
		}
		else
		{
			constexpr int bufferSize = 260;
			char filePath[bufferSize] = "";
			SaveFileWindowsDialog("All Files\0*.*\0", filePath, bufferSize);
			SaveOptions saveOptions;
			serializerManager.Serialize();
			saveOptions.m_FilePath = filePath;
			serializerManager.ProcessForSave(saveOptions);
			saveProcess = 0;
		}

		if (saveProcess == 1)
			ImGui::Text("Saving process is failed because when you choose type you select \'None\'");
		saveThreadRunning = false;
	}
}