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

	static std::string serializationTypeStatus = "Did not saved yet.";
	static std::string binarySerializationStatus = "Did not saved yet.";
	static std::string saveOptionsStatus = "Did not saved yet.";
	static std::string saveStatus = "Did not saved yet.";

	if(ImGui::Button(v_SaveButtonText.c_str()))
	{
		if (!saveThreadRunning)
		{
			saveThreadRunning = true;
			std::thread v_SaveWorker(SaveOperation, std::ref(v_CombineInfos), std::ref(saveThreadRunning), std::ref(serializationTypeStatus), std::ref(binarySerializationStatus), std::ref(saveOptionsStatus), std::ref(saveStatus));
			v_SaveWorker.detach();
		}
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
		if (!loadThreadRunning)
		{
			loadThreadRunning = true;
			std::thread v_LoadWorker(LoadOperation, std::ref(v_CombineInfos), std::ref(loadThreadRunning));
			v_LoadWorker.detach();
		}
	}

	if (s_IsSaveDisabled)
		ImGui::EndDisabled();



	//DeserializeSpec deserializerSpecialization;
	//static char m_filePath[256];
	//static bool m_XorFilter = false;
	//static bool m_Compression = false;
	//static bool m_Artifact = false;
	//static int32_t m_XorKey = 0;
	//static int32_t m_ComboSelected = 0;
	//static int32_t m_BeginVisualization = 0;
	//static int32_t m_EndVisualization = 0;
	//ImGui::InputText("##filepath", m_filePath, IM_ARRAYSIZE(m_filePath));
	//ImGui::SameLine();
	//if (ImGui::Button("Browse")) {

	//}
	//ImGui::InputScalar("Begin Index", ImGuiDataType_U32, &m_BeginVisualization, NULL, NULL, "%d");
	//ImGui::InputScalar("Last Index", ImGuiDataType_U32, &m_EndVisualization, NULL, NULL, "%d");

	//const char* buffer[2] = { "JSON" , "BINARY" };
	//ImGui::Checkbox("Xor Filter", &m_XorFilter);
	//if (m_XorFilter) {
	//	ImGui::SameLine(NULL, 15.0f);
	//	ImGui::InputInt("Xor Key", &m_XorKey);
	//}
	//ImGui::Checkbox("Compression", &m_Compression);
	//ImGui::Checkbox("Artifact", &m_Artifact);
	//if (m_Artifact) {
	//	ImGui::SameLine(NULL, 15.0f);
	//	ImGui::Combo("##artifact", &m_ComboSelected, buffer, IM_ARRAYSIZE(buffer));
	//}

	//if (ImGui::Button("Load")) {
	//	
	//	deserializerSpecialization.m_SaveOptions.m_FilePath = m_filePath;
	//	deserializerSpecialization.m_FileType = Enum_DeserializeContentType(m_ComboSelected + 1);
	//	deserializerSpecialization.m_SaveOptions.m_ArtifactType = ArtifactType(m_ComboSelected);
	//	deserializerSpecialization.m_SaveOptions.m_XorKey = m_XorKey;
	//	if (m_XorFilter)
	//		deserializerSpecialization.m_SaveOptions.m_SaveFlags = (Enum_Save)(deserializerSpecialization.m_SaveOptions.m_SaveFlags | Enum_Save::E_XorFilter);
	//	if (m_Artifact)
	//		deserializerSpecialization.m_SaveOptions.m_SaveFlags = (Enum_Save)(deserializerSpecialization.m_SaveOptions.m_SaveFlags | Enum_Save::E_CreateArtifact);

	//	if (m_Compression)
	//		deserializerSpecialization.m_SaveOptions.m_SaveFlags = (Enum_Save)(deserializerSpecialization.m_SaveOptions.m_SaveFlags | Enum_Save::E_Compress);
	//	//else
	//	//	deserializerSpecialization.m_SaveOptions.m_SaveFlags = (Enum_Save)(deserializerSpecialization.m_SaveOptions.m_SaveFlags | Enum_Save::E_Decompress);


	//	DeserializerManager deserializerManager(deserializerSpecialization);
	//	deserializerManager.Deserialize();
	//	v_CombineInfos = deserializerManager.GetCombineInfos();
	//
	//	while (m_BeginVisualization <= m_EndVisualization) {
	//		v_CombineInfos[(m_BeginVisualization - 1)].GetCombineInfoStatusRef() = true;
	//		for (int32_t criteriaIterator = 0; criteriaIterator< v_CombineInfos[m_BeginVisualization -1].GetCombineCriteriasRef().size(); criteriaIterator++) {
	//			v_CombineInfos[(m_BeginVisualization - 1)].GetCombineCriteriasRef()[criteriaIterator].GetCombineCriteriaStatus() = true;
	//			for (int32_t sourceCriteriaIterator = 0; sourceCriteriaIterator < v_CombineInfos[m_BeginVisualization - 1].GetCombineCriteriasRef()[criteriaIterator].GetSourceCriteriasRef().size(); sourceCriteriaIterator++) {
	//				v_CombineInfos[m_BeginVisualization - 1].GetCombineCriteriasRef()[criteriaIterator].GetSourceCriteriasRef()[sourceCriteriaIterator].GetSourceCriteriaStatusRef() = true;
	//			}
	//		}
	//		m_BeginVisualization++;
	//	}
	//	m_BeginVisualization--;
	//}

	ImGui::End();


	//ImGui::ShowDemoWindow();
	//Render();
}