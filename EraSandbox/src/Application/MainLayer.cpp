#include "MainLayer.h"
#include "CombineInfoRoot.h"
#include "ImGui/ApplicationUtils.h"
#include "ImGui/SaveOptionsRender.h"
#include "ImGui/LoadOptionsRender.h"

#include <thread>

using namespace ApplicationUtils;

void MainLayer::OnUIRender() 
{
	static bool saveThreadRunning = false;
	static bool loadThreadRunning = false;

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
	int32_t inputBufferSize = 0;

	ApplicationUtils::DrawSaveOptions();

	std::string v_SaveButtonText = "";
	static bool s_IsSaveDisabled = false;
	if (saveThreadRunning)
	{
		v_SaveButtonText = "Saving..";
		ImGui::BeginDisabled();
		s_IsSaveDisabled = true;
	}
	else
	{
		s_IsSaveDisabled = false;
		v_SaveButtonText = "Save";
	}

	// YOU NEED TO CHANGE THE STRINGS BECAUSE EVERYTHING SHOULDNT BE STATIC (HEAP MEM ALLOC OR EDIT THE SCOPE)
	static std::string serializationTypeStatus = "Did not saved yet.";
	static std::string binarySerializationStatus = "Did not saved yet.";
	static std::string saveOptionsStatus = "Did not saved yet.";
	static std::string saveStatus = "Did not saved yet.";

	if(ImGui::Button(v_SaveButtonText.c_str()))
	{
		ImGui::OpenPopup("Save");
	}
	if (ImGui::BeginPopupModal("Save", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Do you really want to save this data ?");
		ImGui::Text("IF YOU CLICK YES, IT MAY OVERWRITE ANOTHER FILE WITH THE SAME NAME!");
		ImGui::Separator();
		if (ImGui::Button("Yes")) {
			if (!saveThreadRunning)
			{
				saveThreadRunning = true;
				std::thread v_SaveWorker(SaveOperation, std::ref(v_CombineInfos), std::ref(saveThreadRunning), std::ref(serializationTypeStatus), std::ref(binarySerializationStatus), std::ref(saveOptionsStatus), std::ref(saveStatus), std::ref(inputBufferSize));
				v_SaveWorker.detach();
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("No")) {
			saveThreadRunning = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (s_IsSaveDisabled)
		ImGui::EndDisabled();

	for (int32_t statusIterator = 0; statusIterator < 4; statusIterator++)
	{
		ImGui::NewLine();
		switch (statusIterator)
		{
		case 0:
			ImGui::Text("%s : %s", "Serialization Type Status", serializationTypeStatus.c_str());
			break;
		case 1:
			ImGui::Text("%s : %s", "Binary Serialization Status", binarySerializationStatus.c_str());
			break;
		case 2:
			ImGui::Text("%s : %s", "Save Options Status", saveOptionsStatus.c_str());
			break;
		case 3:
			ImGui::Text("%s : %s", "Save Status", saveStatus.c_str());
			break;
		default:
			ImGui::Text("The code should not be here!!!");
			ImGui::NewLine();
			break;
		}
	}

	ImGui::End();

	ImGui::Begin("Load Options");
	DrawLoadOptions();
	std::string v_LoadButtonText = "";
	static bool s_IsLoadDisabled = false;
	
	if (loadThreadRunning)
	{
		v_LoadButtonText = "Loading..";
		ImGui::BeginDisabled();
		s_IsLoadDisabled = true;
	}
	else
	{
		s_IsLoadDisabled = false;
		v_LoadButtonText = "Load";
	}

	if (ImGui::Button(v_LoadButtonText.c_str()))
	{
		ImGui::OpenPopup("Load");
	}

	if (ImGui::BeginPopupModal("Load", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Do you really want to load new data ?");
		ImGui::Text("IF YOU CLICK YES YOU WILL LOSE YOUR UNSAVED DATA!");
		ImGui::Separator();
		if (ImGui::Button("Yes")) {
			if (!loadThreadRunning)
			{
				loadThreadRunning = true;
				std::thread v_LoadWorker(LoadOperation, std::ref(v_CombineInfos), std::ref(loadThreadRunning));
				v_LoadWorker.detach();
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("No")) {
			loadThreadRunning = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (s_IsLoadDisabled)
		ImGui::EndDisabled();

	ImGui::End();


	//ImGui::ShowDemoWindow();
	//Render();
}