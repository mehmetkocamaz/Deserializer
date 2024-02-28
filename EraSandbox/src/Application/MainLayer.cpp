#include "MainLayer.h"
#include "CombineInfoRoot.h"
#include "ImGui/ApplicationUtils.h"
#include "ImGui/SaveOptionsRender.h"
#include "ImGui/LoadOptionsRender.h"

#include <thread>

using namespace ApplicationUtils;

void MainLayer::OnAttach() {
	bool autoSaveCheckBox = false;
	std::thread v_AutoSaveWorker(&ApplicationUtils::AutoSaveOperation, std::ref(m_AutoSaveCheck));
	v_AutoSaveWorker.detach();
}

void MainLayer::OnUIRender()
{

	static bool saveThreadRunning = false;
	static bool loadThreadRunning = false;
	static bool autoSaveThreadRunning = false;
	static bool autoSaveCheckBox = false;
	auto startTime = std::chrono::steady_clock::now();
	static auto endTime = startTime + std::chrono::minutes(2);

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
	static std::string savePathErrorText = "";
	static std::string saveNameErrorText = "";
	bool isSavePathEmpty = true;
	bool isSaveNameEmpty = true;
	ApplicationUtils::DrawSaveOptions(savePathErrorText, saveNameErrorText);

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
		if (savePathErrorText.size() > 3) {
			isSavePathEmpty = true;
		}
		else if (saveNameErrorText.size() > 3) {
			isSaveNameEmpty = true;
		}
		else
		{
			isSavePathEmpty = false;
			isSaveNameEmpty = false;

			ImGui::OpenPopup("Save");
		}
	}
	ImGui::NewLine();
	if (isSavePathEmpty)
		ImGui::Text(savePathErrorText.c_str());
	if(isSaveNameEmpty)
		ImGui::Text(saveNameErrorText.c_str());


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
			ImGui::Text("%s ", "Serialization Type Status");
			ImGui::SameLine(220.0f, NULL);
			ImGui::Text(": %s", serializationTypeStatus.c_str());
			break;
		case 1:
			ImGui::Text("%s ", "Binary Serialization Status");
			ImGui::SameLine(220.0f, NULL);
			ImGui::Text(": %s", binarySerializationStatus.c_str());
			break;
		case 2:
			ImGui::Text("%s ", "Save Options Status");
			ImGui::SameLine(220.0f, NULL);
			ImGui::Text(": %s", saveOptionsStatus.c_str());
			break;
		case 3:
			ImGui::Text("%s ", "Save Status");
			ImGui::SameLine(220.0f, NULL);
			ImGui::Text(": %s", saveStatus.c_str());
			break;
		default:
			ImGui::Text("The code should not be here!!!");
			ImGui::NewLine();
			break;
		}
	}
	ImGui::End();

	ImGui::Begin("Load Options");
	static int32_t loadStatusFlag = 0;
	static std::string loadErrorText = "";
	bool isLoadPathEmpty = true;
	DrawLoadOptions(loadErrorText);
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
		if(loadErrorText.size() > 3) {
			isLoadPathEmpty = true;
		}
		else
		{
			isLoadPathEmpty = false;
			ImGui::OpenPopup("Load");
		}
	}
	ImGui::NewLine();
	if(isLoadPathEmpty)
		ImGui::Text(loadErrorText.c_str());

	if (ImGui::BeginPopupModal("Load", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Do you really want to load new data ?");
		ImGui::Text("IF YOU CLICK YES YOU WILL LOSE YOUR UNSAVED DATA!");
		ImGui::Separator();
		if (ImGui::Button("Yes")) {
			if (!loadThreadRunning)
			{
				loadThreadRunning = true;
				std::thread v_LoadWorker(LoadOperation, std::ref(v_CombineInfos), std::ref(loadThreadRunning), std::ref(loadStatusFlag));
				v_LoadWorker.detach();
			}
			ImGui::CloseCurrentPopup();
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

	const char* emptyBuffer = "Empty Buffer!";
	const char* decompressionCheck = "Decompression check error!";
	const char* decompressionError = "Decompression error!";
	const char* xorCheck = "Xor check error!";
	const char* xorKey = "Xor key is wrong!";
	const char* fileCannotOpen = "File cannot open!";
	const char* unsupported = "Load unsupported!";

	static std::vector<int32_t> s_ErrorNumbers;
	if(loadStatusFlag > 0) {
		s_ErrorNumbers.clear();
		constexpr int32_t flagSize = 9;
		int32_t tempValue = 256;
		for (int32_t loadFlag = 1; loadFlag <= flagSize; loadFlag++) {
			if (loadStatusFlag >= tempValue) {
				switch (loadFlag)
				{
				case 1:	s_ErrorNumbers.push_back(8);	break;
				case 2: s_ErrorNumbers.push_back(7);	break;
				case 3: s_ErrorNumbers.push_back(6);	break;
				case 4: s_ErrorNumbers.push_back(5);	break;
				case 5: s_ErrorNumbers.push_back(4);	break;
				case 6: s_ErrorNumbers.push_back(3);	break;
				case 7: s_ErrorNumbers.push_back(2);	break;
				case 8: s_ErrorNumbers.push_back(1);	break;
				//default:s_ErrorNumbers.push_back(9);	break;
				}
				loadStatusFlag -= tempValue;
			}
			tempValue /= 2;
		}
	}
	if (s_ErrorNumbers.size() > 0) {
		ImGui::Text("Error List: ");
		ImGui::NewLine();
		for (int32_t errorIterator = 0; errorIterator < s_ErrorNumbers.size(); errorIterator++) {
			switch (s_ErrorNumbers[errorIterator])
			{
			case 1:	ImGui::Text("Load failed!");				break;
			case 2: ImGui::Text("Load unsupported!");			break;
			case 3: ImGui::Text("File cannot opened!");			break;
			case 4: ImGui::Text("Xor key is wrong!");			break;
			case 5: ImGui::Text("Xor check error!");			break;
			case 6: ImGui::Text("Decompression error!");		break;
			case 7: ImGui::Text("Decompression check error!");	break;
			case 8: ImGui::Text("Empty Buffer!");				break;
			}
		}
	}
	ImGui::End();

	ImGui::Begin("Auto Save");
	ImGui::Text("Auto Save: ");
	ImGui::SameLine();
	ImGui::Checkbox("##autosave", &MainLayer::GetAutoSaveCheckRef());
	ImGui::End();


	//ImGui::ShowDemoWindow();
	//Render();
}
