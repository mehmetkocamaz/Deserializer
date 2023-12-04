#pragma once
#include "imgui.h"
#include "Application/CombineInfoRoot.h"

namespace applicationUtils {
	static bool combineCriteriaTrailing = true;
	static bool sourceCriteriasTrailing = true;
	static bool targetRequirementInfoTrailing = true;

	void CombineCriteriaCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, ImGuiTabBarFlags tab_bar_flags);
	void SourceCriteriaCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator);
	void TargetRequirementInfoCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator);
	void SourceRequirementInfoCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator, std::vector<SourceCriteria>& v_SourceCriterias, int32_t& sourceCriteriaIterator);
	void SourceCostInfoCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator, std::vector<SourceCriteria>& v_SourceCriterias, int32_t& sourceCriteriaIterator);
	void SourceProbabiltyInfoCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator, std::vector<SourceCriteria>& v_SourceCriterias, int32_t& sourceCriteriaIterator);

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
				
				ImGui::EndTabBar();
			}
			ImGui::EndTabItem();
		}
		if (!isCombineInfoOpen)
			v_CombineInfo.erase(v_CombineInfo.begin() + combineInfoIterator);
		else
			combineInfoIterator++;
	}


	void CombineCriteriaCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, ImGuiTabBarFlags tab_bar_flags) {
		static ImGuiTabBarFlags tab_bar_flags2 = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
		tab_bar_flags2 = ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);

		for (int32_t combineCriteriaIterator = 0; combineCriteriaIterator < v_CombineInfo[combineInfoIterator].GetCombineCriterias().size(); ) {
			bool isCombineCriteriaOpen = true;
			char combineCriteriaTabName[30];
			snprintf(combineCriteriaTabName, IM_ARRAYSIZE(combineCriteriaTabName), "%s %d", "Combine Criteria", combineCriteriaIterator + 1);

			if (ImGui::BeginTabItem(combineCriteriaTabName, &isCombineCriteriaOpen, ImGuiTabItemFlags_None)) {
				ImGui::Text("Combine Criteria %d", combineCriteriaIterator + 1);

				if (ImGui::BeginTabBar("CombineCriteriaExtensiveTabBar", tab_bar_flags2))
				{
					if (sourceCriteriasTrailing)
						if (ImGui::TabItemButton("+ Source Criteria", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
						{
							SourceCriteria sourceCriteria;
							v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].PushSourceCriterias(sourceCriteria);
						}
					// This was working but need therell be only 1 target req tab. cuz of it well go with the below part.
					//if (targetRequirementInfoTrailing)
					//	if (ImGui::TabItemButton("+ Target Requirement", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
					//		if (v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef().size() < 4) {
					//			RequirementInfo targetRequirementInfo;
					//			v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].PushTargetRequirementInfo(targetRequirementInfo);
					//		}
					//	}

					if (v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfo().size() == 0) {
						RequirementInfo requirementInfo;
						v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].PushTargetRequirementInfo(requirementInfo);
					}

					// target req info creator foo here
					TargetRequirementInfoCreator(v_CombineInfo, combineInfoIterator, combineCriteriaIterator);
					// source criteria creator foo here
					SourceCriteriaCreator(v_CombineInfo, combineInfoIterator, combineCriteriaIterator);

					ImGui::EndTabBar();
				}
				ImGui::EndTabItem();
			}
			if (!isCombineCriteriaOpen)
				v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef().erase(v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef().begin() + combineCriteriaIterator);
			else
				combineCriteriaIterator++;
		}
	}
	void SourceCriteriaCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator) {
		
		static ImGuiTabBarFlags tab_bar_flags3 = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
		tab_bar_flags3 = ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);
		std::vector<SourceCriteria>& v_SourceCriterias = v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetSourceCriteriasRef();
		
		for (int32_t sourceCriteriaIterator = 0; sourceCriteriaIterator < v_SourceCriterias.size();) 
		{
			bool isSourceCriteriaOpen = true;
			char sourceCriteriaName[30];

			snprintf(sourceCriteriaName, IM_ARRAYSIZE(sourceCriteriaName), "%s %d", "Source Criteria", sourceCriteriaIterator + 1);
			if (ImGui::BeginTabItem(sourceCriteriaName, &isSourceCriteriaOpen, ImGuiTabItemFlags_None))
			{
				ImGui::NewLine();
				if (ImGui::Button("Add New Requirement Info")) 
				{
					RequirementInfo requirementInfo;
					if (v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfos().size() < 4)
						v_SourceCriterias[sourceCriteriaIterator].SetSourceRequirementInfo(requirementInfo);
				}
				ImGui::SameLine(NULL, 15.0f);
				if (ImGui::Button("Add New Cost Info"))
				{
					CostInfo costInfo;
					v_SourceCriterias[sourceCriteriaIterator].SetCostInfo(costInfo);
				}
				ImGui::SameLine(NULL, 15.0f);
				if (ImGui::Button("Add New Probabilty Info"))
				{
					ProbabilityInfo probabiltyInfo;
					v_SourceCriterias[sourceCriteriaIterator].SetProbabilityInfo(probabiltyInfo);
				}
				ImGui::NewLine();
				
				ImGui::InputScalar("Source Item Id", ImGuiDataType_U32, &v_SourceCriterias[sourceCriteriaIterator].GetSourceItemIdRef(), NULL, NULL, "%u");
				ImGui::NewLine();
				// The condition to show tab bar. 
				if(v_SourceCriterias[sourceCriteriaIterator].GetCostInfos().size() != 0 || v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfos().size() != 0 || v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfos().size() != 0)
					if (ImGui::BeginTabBar("SourceCriteriaExtensiveTabBar",tab_bar_flags3))
					{

						//bool isSourceRequirementOpen = true;
						//if (ImGui::BeginTabItem("Source Requirements", &isSourceRequirementOpen, ImGuiTabItemFlags_None))
						//{
						//	char comboName[30];
						//	const char* buffer[4] = { "Enchanment" , "Combine" , "Refine", "Socket" };

						//	for (int32_t sourceRequiremenIterator = 0; sourceRequiremenIterator < v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfos().size();)
						//	{
						//		ImGui::Text("\n");
						//		snprintf(comboName, IM_ARRAYSIZE(comboName), "%s %d", "Requirement Type", sourceRequiremenIterator + 1);
						//		if (ImGui::Combo(comboName, (int32_t*)&v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetSourceCriteriasRef()[sourceCriteriaIterator].GetSourceRequirementInfosRef()[sourceRequiremenIterator].m_RequirementType, buffer, IM_ARRAYSIZE(buffer))) {}
						//		char requirementValueName[30];
						//		snprintf(requirementValueName, IM_ARRAYSIZE(requirementValueName), "%s %d", "Requirement Value", sourceRequiremenIterator + 1);
						//		ImGui::InputScalar(requirementValueName, ImGuiDataType_U32, &v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef()[sourceRequiremenIterator].m_RequirementValue, NULL, NULL, "%u");

						//		if (!isSourceRequirementOpen)
						//			v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef().erase(v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef().begin() + sourceRequiremenIterator);
						//		else
						//			sourceRequiremenIterator++;
						//	}
						//	ImGui::EndTabItem();
						//}
						//char costInfoName[16];
						 
						//bool isSourceCostInfoOpen = true;
						//if (ImGui::BeginTabItem("Cost Infos", &isSourceCostInfoOpen, ImGuiTabItemFlags_None))
						//{
						//	ImGui::NewLine();
						//	for(int32_t costInfoIterator = 0; costInfoIterator < v_SourceCriterias[sourceCriteriaIterator].GetCostInfos().size();)
						//	{
						//		char comboName[30];
						//		const char* buffer[4] = { "Silver" , "Billion" , "ContributionPoint" , "BloodPoint" };
						//		snprintf(comboName, IM_ARRAYSIZE(comboName), "%s %d", "Cost Type", costInfoIterator + 1);
						//		if(ImGui::Combo(comboName,(int32_t*)&v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetSourceCriteriasRef()[sourceCriteriaIterator].GetCostInfosRef()[costInfoIterator].m_CostType,buffer,IM_ARRAYSIZE(buffer))) { }
						//		char costValueName[30];
						//		snprintf(costValueName, IM_ARRAYSIZE(costValueName), "%s %d", "Cost Value", costInfoIterator + 1);
						//		ImGui::InputScalar(costValueName, ImGuiDataType_U32, &v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef()[costInfoIterator].m_CostValue, NULL, NULL, "%u");

						//		ImGui::NewLine();

						//		if (!isSourceCostInfoOpen)
						//			v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef().erase(v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef().begin() + costInfoIterator);
						//		else
						//			costInfoIterator++;
						//	}
						//	ImGui::EndTabItem();
						//}
						SourceRequirementInfoCreator(v_CombineInfo, combineInfoIterator, combineCriteriaIterator, v_SourceCriterias, sourceCriteriaIterator);
						SourceCostInfoCreator(v_CombineInfo, combineInfoIterator, combineCriteriaIterator, v_SourceCriterias, sourceCriteriaIterator);
						SourceProbabiltyInfoCreator(v_CombineInfo, combineInfoIterator, combineCriteriaIterator, v_SourceCriterias, sourceCriteriaIterator);
						ImGui::EndTabBar();
					}

				ImGui::EndTabItem();

			}
			if (!isSourceCriteriaOpen)
				v_SourceCriterias.erase(v_SourceCriterias.begin() + sourceCriteriaIterator);
			else
				sourceCriteriaIterator++;
		}
	}
	void TargetRequirementInfoCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator) {
		
		bool isTargetReqOpen = true;
		bool uniqueTargetReq[4] = { false, false, false, false }; // 0 Enchantment, 1 Combine, 2 Refine, 3 Socket
		if (ImGui::BeginTabItem("TargetRequirements", &isTargetReqOpen, ImGuiTabItemFlags_None))
		{
			if (ImGui::Button("Add New Target Requirement"))
			{
				RequirementInfo targetRequirementInfo;
				if(v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfoRef().size() < 4)
					v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].PushTargetRequirementInfo(targetRequirementInfo);
			}
			
			char comboName[30];
			const char* buffer[4] = { "Enchanment" , "Combine" , "Refine", "Socket" };
			
			for (int32_t targetRequirementInfoIterator = 0; targetRequirementInfoIterator < v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfoRef().size();)
			{
				ImGui::Text("\n");
				snprintf(comboName, IM_ARRAYSIZE(comboName), "%s %d", "Requirement Type", targetRequirementInfoIterator +1);
				if (ImGui::Combo(comboName, (int32_t*)&v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfoRef()[targetRequirementInfoIterator].m_RequirementType, buffer, IM_ARRAYSIZE(buffer))) {}
				char requirementValueName[30];
				snprintf(requirementValueName, IM_ARRAYSIZE(requirementValueName), "%s %d", "Requirement Value", targetRequirementInfoIterator + 1);
				ImGui::InputScalar(requirementValueName, ImGuiDataType_U32, &v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetTargetRequirementInfoRef()[targetRequirementInfoIterator].m_RequirementValue, NULL, NULL, "%u");
				
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
		if (ImGui::BeginTabItem("Source Requirements", &isSourceRequirementOpen, ImGuiTabItemFlags_None))
		{
			char comboName[30];
			const char* buffer[4] = { "Enchanment" , "Combine" , "Refine", "Socket" };

			for (int32_t sourceRequiremenIterator = 0; sourceRequiremenIterator < v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfos().size();)
			{
				ImGui::Text("\n");
				snprintf(comboName, IM_ARRAYSIZE(comboName), "%s %d", "Requirement Type", sourceRequiremenIterator + 1);
				if (ImGui::Combo(comboName, (int32_t*)&v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetSourceCriteriasRef()[sourceCriteriaIterator].GetSourceRequirementInfosRef()[sourceRequiremenIterator].m_RequirementType, buffer, IM_ARRAYSIZE(buffer))) {}
				char requirementValueName[30];
				snprintf(requirementValueName, IM_ARRAYSIZE(requirementValueName), "%s %d", "Requirement Value", sourceRequiremenIterator + 1);
				ImGui::InputScalar(requirementValueName, ImGuiDataType_U32, &v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef()[sourceRequiremenIterator].m_RequirementValue, NULL, NULL, "%u");

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
		if (ImGui::BeginTabItem("Cost Infos", &isSourceCostInfoOpen, ImGuiTabItemFlags_None))
		{
			ImGui::NewLine();
			for (int32_t costInfoIterator = 0; costInfoIterator < v_SourceCriterias[sourceCriteriaIterator].GetCostInfos().size();)
			{
				char comboName[30];
				const char* buffer[4] = { "Silver" , "Billion" , "ContributionPoint" , "BloodPoint" };
				//snprintf(costInfoName, IM_ARRAYSIZE(costInfoName), "%s %d", "Cost Info", costInfoIterator + 1);
				snprintf(comboName, IM_ARRAYSIZE(comboName), "%s %d", "Cost Type", costInfoIterator + 1);
				if (ImGui::Combo(comboName, (int32_t*)&v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetSourceCriteriasRef()[sourceCriteriaIterator].GetCostInfosRef()[costInfoIterator].m_CostType, buffer, IM_ARRAYSIZE(buffer))) {}
				char costValueName[30];
				snprintf(costValueName, IM_ARRAYSIZE(costValueName), "%s %d", "Cost Value", costInfoIterator + 1);
				ImGui::InputScalar(costValueName, ImGuiDataType_U32, &v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef()[costInfoIterator].m_CostValue, NULL, NULL, "%u");

				ImGui::NewLine();

				if (!isSourceCostInfoOpen)
					v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef().erase(v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef().begin() + costInfoIterator);
				else
					costInfoIterator++;
			}
			ImGui::EndTabItem();
		}
	}

	void SourceProbabiltyInfoCreator(std::vector<CombineInfo>& v_CombineInfo, int32_t& combineInfoIterator, int32_t& combineCriteriaIterator, std::vector<SourceCriteria>& v_SourceCriterias, int32_t& sourceCriteriaIterator)
	{
		bool isProbabiltyOpen = true;
		if (ImGui::BeginTabItem("Probability Infos", &isProbabiltyOpen, ImGuiTabItemFlags_None))
		{
			ImGui::NewLine();
			for (int32_t probabiltyInfoIterator = 0; probabiltyInfoIterator < v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfos().size();)
			{
				char comboName[30];
				const char* buffer[3] = { "Success" , "Fail" , "Break" };
				snprintf(comboName, IM_ARRAYSIZE(comboName), "%s %d", "Probabilty Type", probabiltyInfoIterator + 1);
				if (ImGui::Combo(comboName, (int32_t*)&v_CombineInfo[combineInfoIterator].GetCombineCriteriasRef()[combineCriteriaIterator].GetSourceCriteriasRef()[sourceCriteriaIterator].GetProbabilityInfosRef()[probabiltyInfoIterator].m_ProbabilityType, buffer, IM_ARRAYSIZE(buffer))) {}
				char costValueName[30];
				snprintf(costValueName, IM_ARRAYSIZE(costValueName), "%s %d", "Probability Value", probabiltyInfoIterator + 1);
				ImGui::InputScalar(costValueName, ImGuiDataType_U32, &v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfosRef()[probabiltyInfoIterator].m_ProbabilityValue, NULL, NULL, "%u");
				ImGui::NewLine();

				if (!isProbabiltyOpen)
					v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfos().erase(v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfos().begin() + probabiltyInfoIterator);
				else
					probabiltyInfoIterator++;
			}
			ImGui::EndTabItem();
		}
	}
}