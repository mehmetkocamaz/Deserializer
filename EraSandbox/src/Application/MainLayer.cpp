#include "MainLayer.h"
#include "CombineInfoRoot.h"
#include "ImGui/ApplicationUtils.h"
#include <thread>

using namespace applicationUtils;

void MainLayer::OnUIRender() 
{
	static bool saveThreadRunning = false;
	ImGui::Begin("Editor");
	std::vector<CombineInfo>& v_CombineInfos = CombineInfoRoot::Instance().m_CombineInfos;

	if (v_CombineInfos.size() == 0) {
		CombineInfo combineInfo;
		combineInfo.GetCombineInfoStatusRef() = true;
		v_CombineInfos.push_back(combineInfo);
	}

	static bool show_leading_button = true;
	static bool show_trailing_button = true;
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
		ImGui::EndTabBar();
	}
	ImGui::End();

	ImGui::Begin("Save Options");
	ImGui::Text("Check before save you did not left anything as \'None\' type");
	if (ImGui::Button("Save"))
	{
		saveThreadRunning = true;
	}
	if (saveThreadRunning)
	{
		// CALL THE THREAD HERE PLS
		std::thread worker(SaveOperation, std::ref(v_CombineInfos), std::ref(saveThreadRunning));
		worker.join();
	}
	ImGui::End();
	
	//ImGui::ShowDemoWindow();
	//Render();

}