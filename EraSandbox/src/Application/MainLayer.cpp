#include "MainLayer.h"
#include "CombineInfoRoot.h"
#include "ImGui/ApplicationUtils.h"

using namespace applicationUtils;

void MainLayer::OnUIRender() 
{
	ImGui::Begin("Editor");

	std::vector<CombineInfo>& v_CombineInfos = CombineInfoRoot::Instance().m_CombineInfos;

	if (v_CombineInfos.size() == 0) {
		CombineInfo combineInfo;
		combineInfo.GetCombineInfoStatusRef() = true;
		v_CombineInfos.push_back(combineInfo);
	}

	static bool show_leading_button = true;  // ? symbol
	static bool show_trailing_button = true; // + symbol 
	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
	tab_bar_flags = ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);

	if (ImGui::BeginTabBar("CombineInfoTabBar", tab_bar_flags))
	{
		if (show_leading_button)
			if (ImGui::TabItemButton("?", ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_NoTooltip))
				ImGui::OpenPopup("HelpMenu");
		if (ImGui::BeginPopup("HelpMenu"))
		{
			ImGui::Text("1-) Press 'New Combine Info' button to add new combine info.\n2-) Press the symbol left to see all the combine infos you add.\n3-) You can close the tabs when hit 'X' symbol.\nWhen you hit that all the data you have about this tab will be deleted.");
			ImGui::EndPopup();
		}

		if (show_trailing_button)
			if (ImGui::TabItemButton("New Combine Info", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
				CombineInfo combineInfo;
				combineInfo.GetCombineInfoStatusRef() = true;
				v_CombineInfos.push_back(combineInfo);
			}

		for (int32_t combineInfoIterator = 0; combineInfoIterator < v_CombineInfos.size(); )
		{
			CombineInfoCreator(v_CombineInfos, combineInfoIterator);
		}
		ImGui::EndTabBar;
	}
	ImGui::End();


	// ***************************************************************************************


	//ImGui::Begin("Showcase");
	//ImGuiTableFlags table_flags = (ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable);
	//int32_t openedCombineInfoIndex = 0;
	//if (ImGui::BeginTable("CombineInfoTable", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable))
	//{
	//	//std::format("Combine Info {0}", (openedCombineInfoIndex + 1).c_str()); // when you want to do format just use it :P
	//	char buffer[30];
	//	snprintf(buffer, IM_ARRAYSIZE(buffer), "%s %d", "Combine Info", (openedCombineInfoIndex + 1));
	//	ImGui::TableSetupColumn(buffer);
	//	ImGui::TableHeadersRow();

	//	ImGui::TableNextColumn();
	//	ImGui::Text("Target Item Id : %d", v_CombineInfos[openedCombineInfoIndex].GetTargetItemId());

	//	for(int32_t criteriaIterator = 0; criteriaIterator < v_CombineInfos[openedCombineInfoIndex].GetCombineCriteriasRef().size(); criteriaIterator++)
	//	{
	//		char buffer[30];
	//		snprintf(buffer, IM_ARRAYSIZE(buffer), "%s %d", "Combine Criteria", (criteriaIterator + 1));

	//		if (ImGui::BeginTable("CombineCriteriaTable", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable))
	//		{
	//			ImGui::TableSetupColumn(buffer);
	//			ImGui::TableHeadersRow();

	//			ImGui::TableNextColumn();
	//			ImGui::Text("Target Requirement Size : %d", v_CombineInfos[openedCombineInfoIndex].GetCombineCriteriasRef()[criteriaIterator].GetTargetRequirementInfoRef().size());
	//			ImGui::Text("Source Criteria Size : %d", v_CombineInfos[openedCombineInfoIndex].GetCombineCriteriasRef()[criteriaIterator].GetSourceCriteriasRef().size());
	//			
	//			if (ImGui::BeginTable("TargetRequirementTable", 2, table_flags))
	//			{
	//				ImGui::TableSetupColumn("Target Requirement Type");
	//				ImGui::TableSetupColumn("Target Requirement Value");
	//				ImGui::TableHeadersRow();
	//				for (int32_t targetRequirementIterator = 0; targetRequirementIterator < v_CombineInfos[openedCombineInfoIndex].GetCombineCriteriasRef()[criteriaIterator].GetTargetRequirementInfoRef().size(); targetRequirementIterator++)
	//				{
	//					ImGui::TableNextRow(ImGuiTableFlags_None);
	//					ImGui::TableNextColumn();
	//					switch (v_CombineInfos[openedCombineInfoIndex].GetCombineCriteriasRef()[criteriaIterator].GetTargetRequirementInfoRef()[targetRequirementIterator].m_RequirementType)
	//					{
	//					case Enum_Requirement::None:
	//						ImGui::Text("None");
	//						break;
	//					case Enum_Requirement::Enchanment:
	//						ImGui::Text("Enchanment");
	//						break;
	//					case Enum_Requirement::Combine:
	//						ImGui::Text("Combine");
	//						break;
	//					case Enum_Requirement::Refine:
	//						ImGui::Text("Refine");
	//						break;
	//					case Enum_Requirement::Socket:
	//						ImGui::Text("Socket");
	//						break;
	//					default:
	//						ImGui::Text("Invalid Choise");
	//						break;
	//					}
	//					ImGui::TableNextColumn();
	//					ImGui::Text("%d", v_CombineInfos[openedCombineInfoIndex].GetCombineCriteriasRef()[criteriaIterator].GetTargetRequirementInfoRef()[targetRequirementIterator].m_RequirementValue);
	//				}
	//				ImGui::EndTable();
	//			}
	//			std::vector<SourceCriteria>& v_SourceCriterias = v_CombineInfos[openedCombineInfoIndex].GetCombineCriteriasRef()[criteriaIterator].GetSourceCriteriasRef();
	//			
	//			for(int32_t sourceCriteriaIterator = 0; sourceCriteriaIterator < v_SourceCriterias.size(); sourceCriteriaIterator++)
	//			{
	//				char buffer[30];
	//				snprintf(buffer, IM_ARRAYSIZE(buffer), "%s %d", "Source Criteria", (sourceCriteriaIterator + 1));
	//				if (ImGui::BeginTable(buffer, 1, table_flags))
	//				{
	//					ImGui::TableSetupColumn(buffer);
	//					ImGui::TableHeadersRow();
	//					ImGui::TableNextColumn();
	//					ImGui::Text("Source Item Id : %d", v_SourceCriterias[sourceCriteriaIterator].GetSourceItemId());
	//					if(v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef().size() != 0)
	//						if (ImGui::BeginTable("SourceRequirementTable", 2, table_flags))
	//						{
	//							ImGui::TableSetupColumn("Source Requirement Type");
	//							ImGui::TableSetupColumn("Source Requirement Value");
	//							ImGui::TableHeadersRow();

	//							for (int32_t sourceRequirementIterator = 0; sourceRequirementIterator < v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef().size(); sourceRequirementIterator++)
	//							{
	//								ImGui::TableNextRow(ImGuiTableFlags_None);
	//								ImGui::TableNextColumn();
	//								switch (v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef()[sourceRequirementIterator].m_RequirementType)
	//								{
	//								case Enum_Requirement::None:
	//									ImGui::Text("None");
	//									break;
	//								case Enum_Requirement::Enchanment:
	//									ImGui::Text("Enchanment");
	//									break;
	//								case Enum_Requirement::Combine:
	//									ImGui::Text("Combine");
	//									break;
	//								case Enum_Requirement::Refine:
	//									ImGui::Text("Refine");
	//									break;
	//								case Enum_Requirement::Socket:
	//									ImGui::Text("Socket");
	//									break;
	//								default:
	//									ImGui::Text("Invalid Choise");
	//									break;
	//								}
	//								ImGui::TableNextColumn();
	//								ImGui::Text("%d", v_SourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef()[sourceRequirementIterator].m_RequirementValue);
	//							}
	//							ImGui::EndTable();
	//						}

	//					if (v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef().size() != 0)
	//						if (ImGui::BeginTable("SourceCostTable", 2, table_flags))
	//						{
	//							ImGui::TableSetupColumn("Source Cost Type");
	//							ImGui::TableSetupColumn("Source Cost Value");
	//							ImGui::TableHeadersRow();
	//							for (int32_t sourceCostIterator = 0; sourceCostIterator < v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef().size(); sourceCostIterator++)
	//							{
	//								ImGui::TableNextRow(ImGuiTableFlags_None);
	//								ImGui::TableNextColumn();
	//								switch (v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef()[sourceCostIterator].m_CostType)
	//								{
	//								case Enum_Cost::None:
	//									ImGui::Text("None");
	//									break;
	//								case Enum_Cost::Billion:
	//									ImGui::Text("Billion");
	//									break;
	//								case Enum_Cost::BloodPoint:
	//									ImGui::Text("BloodPoint");
	//									break;
	//								case Enum_Cost::ContributionPoint:
	//									ImGui::Text("ContributionPoint");
	//									break;
	//								case Enum_Cost::Silver:
	//									ImGui::Text("Silver");
	//									break;
	//								default:
	//									ImGui::Text("Invalid Choise");
	//									break;
	//								}
	//								ImGui::TableNextColumn();
	//								ImGui::Text("%d", v_SourceCriterias[sourceCriteriaIterator].GetCostInfosRef()[sourceCostIterator].m_CostValue);
	//							}
	//							ImGui::EndTable();
	//						}

	//					if (v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfosRef().size() != 0)
	//						if (ImGui::BeginTable("SourceProbabilityInfo", 2, table_flags))
	//						{
	//							ImGui::TableSetupColumn("Source Probability Type");
	//							ImGui::TableSetupColumn("Source Probability Value");
	//							ImGui::TableHeadersRow();
	//							for (int32_t sourceProbabilityIterator = 0; sourceProbabilityIterator < v_SourceCriterias[criteriaIterator].GetProbabilityInfosRef().size(); sourceProbabilityIterator++)
	//							{
	//								ImGui::TableNextRow(ImGuiTableFlags_None);
	//								ImGui::TableNextColumn();
	//								switch (v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfosRef()[sourceProbabilityIterator].m_ProbabilityType)
	//								{
	//								case Enum_Probability::Success:
	//									ImGui::Text("Success");
	//									break;
	//								case Enum_Probability::Fail:
	//									ImGui::Text("Fail");
	//									break;
	//								case Enum_Probability::Break:
	//									ImGui::Text("Break");
	//									break;
	//								default:
	//									ImGui::Text("Invalid Choise");
	//									break;
	//								}
	//								ImGui::TableNextColumn();
	//								ImGui::Text("%d", v_SourceCriterias[sourceCriteriaIterator].GetProbabilityInfosRef()[sourceProbabilityIterator].m_ProbabilityValue);
	//							}
	//							ImGui::EndTable();
	//						}
	//					ImGui::EndTable();
	//				}
	//			}
	//			ImGui::EndTable();
	//		}
	//	}
	//	ImGui::EndTable();
	//}
	//ImGui::End();


	// ***************************************************************************************

	//ImGui::ShowDemoWindow();
	//Render();

}