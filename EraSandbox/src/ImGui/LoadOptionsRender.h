#pragma once
#include "imgui.h"
#include <iostream>
#include <Windows.h>
//#include <shobjidl.h> 
#include <ShObjIdl.h>

namespace ApplicationUtils 
{
	struct FileLoadOptions
	{
		static constexpr size_t s_BufferSize = 256;
		PWSTR m_Pwstr{};
		char m_InputBuffer[s_BufferSize] = "";
		bool m_XorFilterCheck = false;
		bool m_DecompressionCheck = false;
		int32_t m_XorValue = 0;
		int32_t m_FirstIndex = 0;
		int32_t m_LastIndex = 0;
		bool m_PartitionCheck = false;

		SaveOptions TranspileToLoadOptions() {
			SaveOptions v_LoadOptions{};
			v_LoadOptions.m_FilePath = std::filesystem::path(m_InputBuffer) /*/ std::filesystem::path(m_FileNameBuffer)*/;

			if (m_DecompressionCheck)
				v_LoadOptions.m_SaveFlags = (Enum_Save)(v_LoadOptions.m_SaveFlags | E_Decompress);

			if (m_XorFilterCheck)
			{
				v_LoadOptions.m_SaveFlags = (Enum_Save)(v_LoadOptions.m_SaveFlags | E_XorFilter);
				v_LoadOptions.m_XorKey = m_XorValue;
			}

			v_LoadOptions.m_ArtifactType = BINARY;
			return v_LoadOptions;
		}
	};

	static FileLoadOptions s_FileLoadOptions;

	void LoadOperation(std::vector<CombineInfo>& v_CombineInfos, bool& loadThreadRunning) {
		DeserializeSpec deserializerSpecialization;
		deserializerSpecialization.m_FileType = Enum_DeserializeContentType::BINARY;
		deserializerSpecialization.m_SaveOptions = s_FileLoadOptions.TranspileToLoadOptions();
		DeserializerManager deserializerManager(deserializerSpecialization);
		deserializerManager.Deserialize();
		v_CombineInfos = deserializerManager.GetCombineInfos();

		if (s_FileLoadOptions.m_FirstIndex <= 0)
			s_FileLoadOptions.m_FirstIndex = 1;
		if (s_FileLoadOptions.m_LastIndex > v_CombineInfos.size())
			s_FileLoadOptions.m_LastIndex = v_CombineInfos.size();

		int32_t combineInfoIterator = s_FileLoadOptions.m_FirstIndex - 1;
		if (s_FileLoadOptions.m_PartitionCheck) {
			if(s_FileLoadOptions.m_LastIndex <= v_CombineInfos.size())
				while (combineInfoIterator <= (s_FileLoadOptions.m_LastIndex - 1)) {
					v_CombineInfos[combineInfoIterator].GetCombineInfoStatusRef() = true;
					for (int32_t criteriaIterator = 0; criteriaIterator < v_CombineInfos[combineInfoIterator].GetCombineCriteriasRef().size(); criteriaIterator++) {
						v_CombineInfos[combineInfoIterator].GetCombineCriteriasRef()[criteriaIterator].GetCombineCriteriaStatus() = true;
						for (int32_t sourceCriteriaIterator = 0; sourceCriteriaIterator < v_CombineInfos[combineInfoIterator].GetCombineCriteriasRef()[criteriaIterator].GetSourceCriteriasRef().size(); sourceCriteriaIterator++) {
							v_CombineInfos[combineInfoIterator].GetCombineCriteriasRef()[criteriaIterator].GetSourceCriteriasRef()[sourceCriteriaIterator].GetSourceCriteriaStatusRef() = true;
						}
					}
					combineInfoIterator++;
				}
		}
		else {
			for (auto& combineInfo : v_CombineInfos) {
				combineInfo.GetCombineInfoStatusRef() = true;
				for (auto& combineCriteria : combineInfo.GetCombineCriteriasRef()) {
					combineCriteria.GetCombineCriteriaStatus() = true;
					for (auto& sourceCriteria : combineCriteria.GetSourceCriteriasRef()) {
						sourceCriteria.GetSourceCriteriaStatusRef() = true;
					}
				}
			}
		}
		loadThreadRunning = false;
	}

	void LoadFileDialog(PWSTR& pwsz)
	{
		IFileDialog* pfd;
		HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

		if (SUCCEEDED(hr))
		{
			// Set options to force the file system
			DWORD dwOptions;
			if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
			{
				pfd->SetOptions(dwOptions | FOS_FORCEFILESYSTEM);
			}

			// Add file type filters
			COMDLG_FILTERSPEC fileTypes[] =
			{
				{ L"ERA Files", L"*.ERA" },
				{ L"Binary Files", L"*.bin" },
				{ L"All Files", L"*.*" } // Optional: All files filter
			};
			hr = pfd->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);

			// Show the dialog
			if (SUCCEEDED(hr))
			{
				hr = pfd->Show(NULL);
				if (SUCCEEDED(hr))
				{
					IShellItem* psi;
					hr = pfd->GetResult(&psi);
					if (SUCCEEDED(hr))
					{
						PWSTR pszPath;
						hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
						if (SUCCEEDED(hr))
						{
							pwsz = pszPath;
						}
						psi->Release();
					}
				}
			}

			pfd->Release();
		}
	}



	
	void DrawLoadOptions(std::string& v_LoadErrorText) {
		ImGui::Text("Path to Load:");
		ImGui::SameLine(NULL, 31.0f);
		ImGui::InputText("##inputLoad", s_FileLoadOptions.m_InputBuffer, s_FileLoadOptions.s_BufferSize);
		if (strlen(s_FileLoadOptions.m_InputBuffer) < 3) {
			v_LoadErrorText = "The path cannot be empty!";
		}
		else
			v_LoadErrorText.clear();
		ImGui::SameLine();

		if (ImGui::Button("Browse")) {
			constexpr int bufferSize = 260;
			char filePath[bufferSize] = "";
			CoInitialize(NULL);
			CoTaskMemFree(s_FileLoadOptions.m_Pwstr);
			LoadFileDialog(s_FileLoadOptions.m_Pwstr);
			strncpy(s_FileLoadOptions.m_InputBuffer, PWSTRToStdString(s_FileLoadOptions.m_Pwstr).c_str(), s_FileLoadOptions.s_BufferSize - 1);
			CoUninitialize();
		}

		ImGui::Text("Xor Filter:");
		ImGui::SameLine(NULL, 59.0f);
		ImGui::Checkbox("##xorfilterLoad", &s_FileLoadOptions.m_XorFilterCheck);
		if (s_FileLoadOptions.m_XorFilterCheck) {
			ImGui::SameLine();
			ImGui::InputInt("##xorvalueLoad", &s_FileLoadOptions.m_XorValue);
		}

		ImGui::Text("Decompression:");
		ImGui::SameLine();
		ImGui::Checkbox("##compressionLoad", &s_FileLoadOptions.m_DecompressionCheck);
		
		ImGui::Text("Partition:");
		ImGui::SameLine(NULL, 63.0f);
		ImGui::Checkbox("##partition", &s_FileLoadOptions.m_PartitionCheck);
		if (s_FileLoadOptions.m_PartitionCheck) {
			ImGui::Text("First Index:");
			ImGui::SameLine(NULL, 48.0f);
			ImGui::InputInt("##startIndexLoad", &s_FileLoadOptions.m_FirstIndex);

			ImGui::Text("Last Index:");
			ImGui::SameLine(NULL, 49.0f);
			ImGui::InputInt("##lastIndexLoad", &s_FileLoadOptions.m_LastIndex);
		}
	}
}