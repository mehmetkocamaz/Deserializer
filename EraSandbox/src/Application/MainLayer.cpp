#include "MainLayer.h"
#include "CombineInfoRoot.h"

void MainLayer::OnUIRender(){
	ImGui::Begin("Editor");
	//ImGui::ShowDemoWindow();

	std::vector<CombineInfo>& v_CombineInfos = CombineInfoRoot::Instance().m_CombineInfos;

	if (v_CombineInfos.size() == 0) {
		CombineInfo combineInfo;
		v_CombineInfos.push_back(combineInfo);
	}
	//static std::vector<int32_t> selected;
	//static std::vector<uint32_t> requirementValues;
 
	static bool show_leading_button = true;  // ? symbol
	static bool show_trailing_button = true; // + symbol 
	static ImVector<int32_t> activeSourceCriteriaTabs;
	static ImVector<int32_t> activeTargetRequirementTabs;
	static uint32_t nextTargetTabId;
	static uint32_t nextSourceTabId;
	static bool combineCriteriaTrailing = true;
	static bool targetRequirementTrailing = true;
	static bool sourceCriteriasTrailing = true;
	static bool requirementTrailing = true;

	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
	tab_bar_flags = ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);

	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		if (show_leading_button)
			if (ImGui::TabItemButton("?", ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_NoTooltip))
				ImGui::OpenPopup("MyHelpMenu");
		if (ImGui::BeginPopup("MyHelpMenu"))
		{
			ImGui::Text("1-) Press 'New Combine Info' button to add new combine info.\n2-) Press the symbol left to see all the combine infos you add.\n3-) You can close the tabs when hit 'X' symbol.\nWhen you hit that all the data you have about this tab will be deleted.");
			ImGui::EndPopup();
		}

		if (show_trailing_button)
			if (ImGui::TabItemButton("New Combine Info", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
				CombineInfo combineInfo;
				v_CombineInfos.push_back(combineInfo);
			}

		for (int n = 0; n < v_CombineInfos.size(); )
		{
			bool open = true;
			char name[16];
			snprintf(name, IM_ARRAYSIZE(name), "%s %d", "Combine Info", n + 1);
			if (ImGui::BeginTabItem(name, &open, ImGuiTabItemFlags_None))
			{
				static ImGuiTabBarFlags tab_bar_flags2 = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
				tab_bar_flags2 = ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);

				ImGui::Text("%s:", name);
				ImGui::InputScalar("Target Item Id", ImGuiDataType_U32, &v_CombineInfos[n].GetTargetItemIdRef(), NULL, NULL, "%u");

				if (ImGui::BeginTabBar("CombineCriteriasTabBar", tab_bar_flags2)) {
					
					if (combineCriteriaTrailing)
						if (ImGui::TabItemButton("+ Combine Criteria", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
							CombineCriteria combineCriteria;
							v_CombineInfos[n].PushCombineCriterias(combineCriteria);
						}

					
					for (int32_t i = 0; i < v_CombineInfos[n].GetCombineCriterias().size(); )
					{
						bool open2 = true;
						char name2[30];
						snprintf(name2, IM_ARRAYSIZE(name2), "%s %d", "Combine Criteria", i + 1);

						if (ImGui::BeginTabItem(name2 , &open2, ImGuiTabItemFlags_None))
						{
							ImGui::Text("Combine Criteria %d", i + 1);

							if (ImGui::BeginTabBar("CombCritExtensiveTabBar",tab_bar_flags)) {
								if (sourceCriteriasTrailing)
									if (ImGui::TabItemButton("+ Source Criteria",ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
									{
										SourceCriteria sourceCriteria;
										v_CombineInfos[n].GetCombineCriteriasRef()[i].PushSourceCriterias(sourceCriteria);
									}

								if (targetRequirementTrailing)
								{
									if (ImGui::TabItemButton("+ Target Requirement", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
									{
										if (v_CombineInfos[n].GetCombineCriteriasRef()[i].GetTargetRequirementInfo().size() < 4)
										{
											RequirementInfo targetRequirementInfo;
											v_CombineInfos[n].GetCombineCriteriasRef()[i].PushTargetRequirementInfo(targetRequirementInfo);
										}
									}
								}


								for (int32_t k = 0; k < v_CombineInfos[n].GetCombineCriterias()[i].GetSourceCriterias().size(); )
								{
									bool openSourceCriteria = true;
									char name3[30];
									snprintf(name3, IM_ARRAYSIZE(name3), "%s %d", "Source Criteria", k + 1);

									if (ImGui::BeginTabItem(name3, &openSourceCriteria, ImGuiTabItemFlags_None)) {
										ImGui::EndTabItem();
									}
									if (!openSourceCriteria)
										v_CombineInfos[n].GetCombineCriteriasRef()[i].GetSourceCriteriasRef().erase(v_CombineInfos[n].GetCombineCriteriasRef()[i].GetSourceCriteriasRef().begin() + k);
									else
										k++;
								}

								uint32_t v_Counter = 0;
								std::vector<RequirementInfo>& v_TargetRequirementInfos = v_CombineInfos[n].GetCombineCriteriasRef()[i].GetTargetRequirementInfoRef();
								for (RequirementInfo& v_TargetRequirementInfo : v_TargetRequirementInfos)
								{
									RequirementInfo targetRequirementInfo;
									bool openTargetReq = true;
									char name3[30];
									const char* buffer[4] = { "Enchanment" , "Combine" , "Refine", "Socket" };
									snprintf(name3, IM_ARRAYSIZE(name3), "%s %s %d", buffer[(uint32_t)v_TargetRequirementInfo.m_RequirementType], " Info", v_Counter + 1);

									if (ImGui::BeginTabItem(name3, &openTargetReq, ImGuiTabItemFlags_None)) {

										ImGui::Text("Requirement Type:\n");
										char comboName[30];
										const char* buffer[4] = { "Enchanment" , "Combine" , "Refine", "Socket" };
										snprintf(comboName, IM_ARRAYSIZE(comboName), "%s %d", "Requirement Type", v_Counter + 1);

										if (ImGui::Combo(comboName, (int32_t*)&v_TargetRequirementInfo.m_RequirementType, buffer, IM_ARRAYSIZE(buffer))) {}

										char reqValue[30];
										snprintf(reqValue, IM_ARRAYSIZE(reqValue), "%s %d", "Requirement Value", v_Counter + 1);
										ImGui::InputScalar(reqValue, ImGuiDataType_U32, &v_TargetRequirementInfo.m_RequirementValue, NULL, NULL, "%u");

										ImGui::EndTabItem();
									}
									if (!openTargetReq)
										v_TargetRequirementInfos.erase(v_CombineInfos[n].GetCombineCriteriasRef()[i].GetTargetRequirementInfoRef().begin() + v_Counter);
									else
										++v_Counter;
								}

								ImGui::EndTabBar();
							}
							ImGui::EndTabItem();
						}
						if (!open2)
							v_CombineInfos[n].GetCombineCriteriasRef().erase(v_CombineInfos[n].GetCombineCriteriasRef().begin() + i);
						else
							i++;
					}	

					ImGui::EndTabBar();
				}
				ImGui::EndTabItem();
			}

			if (!open) {
				v_CombineInfos.erase(v_CombineInfos.begin() + n);
			}
			else
				n++;
		}

		ImGui::EndTabBar();
	}


	ImGui::End();

	//Render();
	}