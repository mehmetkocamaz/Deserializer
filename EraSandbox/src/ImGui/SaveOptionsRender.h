#pragma once
#include "imgui.h"
#include <iostream>
#include <windows.h>
#include <shobjidl.h> 
#include <cstring>

//#include <direct.h>
#include <filesystem>
namespace fs = std::filesystem; // Namespace kýsaltmasý yapýlmayacak

#include "Application/CombineInfoRoot.h"


#include <sys/stat.h>
#include <cstdlib>
#include <chrono>
#include <ctime>

namespace ApplicationUtils
{

	struct FileSaveOptions
	{
		static constexpr size_t s_BufferSize = 256;
		PWSTR m_Pwstr{};
		char m_InputBuffer[s_BufferSize] = "";
		char m_FileNameBuffer[s_BufferSize] = "CombineInfo.ERA";
		bool m_XorFilterCheck = false;
		bool m_CompressionCheck = false;
		bool m_ArtifactCheck = false;
		int32_t m_ArtifactSelected = 0;
		int32_t m_XorValue = 0;

		SaveOptions TranspileToSaveOptions()
		{
			SaveOptions v_SaveOptions{};
			v_SaveOptions.m_FilePath = std::filesystem::path(m_InputBuffer) / std::filesystem::path(m_FileNameBuffer);

			if (m_CompressionCheck)
				v_SaveOptions.m_SaveFlags = (Enum_Save)(v_SaveOptions.m_SaveFlags | E_Compress);
			
			if (m_XorFilterCheck) {
				v_SaveOptions.m_SaveFlags = (Enum_Save)(v_SaveOptions.m_SaveFlags | E_XorFilter);
				v_SaveOptions.m_XorKey = m_XorValue;
			}
			
			if (m_ArtifactCheck) {
				v_SaveOptions.m_SaveFlags = (Enum_Save)(v_SaveOptions.m_SaveFlags | E_CreateArtifact);
				v_SaveOptions.m_ArtifactType = (ArtifactType)m_ArtifactSelected;
			}

			return v_SaveOptions;
		}
	};

	static FileSaveOptions s_FileSaveOptions;
	static FileSaveOptions s_FileAutoSaveOptions;

	void SaveOperation(std::vector<CombineInfo>& v_CombineInfos, bool& saveThreadRunning, std::string& serializationTypeStatus, std::string& binarySerializationStatus, std::string& saveOptionsStatus, std::string& saveStatus, int32_t& inputBufferSize)
	{
		SerializeSpec spec{};
		spec.m_ContentType = Enum_SerizalizeContentType::BINARY;
		spec.m_CombineInfos = &v_CombineInfos;
		SerializerManager serializerManager(spec);
		if (serializerManager.CheckForNone() == Enum_SerializationStatus::TYPE_NONE)
		{
			saveProcess = 1;
		}
		else
		{
			serializerManager.Serialize();
			serializerManager.ProcessForSave(s_FileSaveOptions.TranspileToSaveOptions());
		}

		//if (saveProcess == 1)
		//	ImGui::Text("Saving process is failed because when you choose type you select \'None\'");

		saveThreadRunning = false;
		inputBufferSize = strlen(s_FileSaveOptions.m_InputBuffer);
		if(inputBufferSize > 2)
			for (int32_t statusIterator = 0; statusIterator < serializerManager.GetSerializationStatusRef().size(); statusIterator++)
			{
				//std::cout << serializationTypeStatus << std::endl;
				switch (serializerManager.GetSerializationStatusRef()[statusIterator])
				{
				case Enum_SerializationStatus::SUCCESS:
					statusIterator == 0 ? serializationTypeStatus = "Success" : (statusIterator == 1 ? binarySerializationStatus = "Success" : (statusIterator == 2 ? saveOptionsStatus = "Success" : (statusIterator == 3 ? saveStatus = "Success" : "Error")));
					break;
				case Enum_SerializationStatus::COMPRESS_FAIL:
					statusIterator == 0 ? serializationTypeStatus = "Compress Fail" : (statusIterator == 1 ? binarySerializationStatus = "Compress Fail" : (statusIterator == 2 ? saveOptionsStatus = "Compress Fail" : (statusIterator == 3 ? saveStatus = "Compress Fail" : "Error")));
					break;
				case Enum_SerializationStatus::EMPTY_BUFFER:
					statusIterator == 0 ? serializationTypeStatus = "Empty Buffer" : (statusIterator == 1 ? binarySerializationStatus = "Empty Buffer" : (statusIterator == 2 ? saveOptionsStatus = "Empty Buffer" : (statusIterator == 3 ? saveStatus = "Empty Buffer" : "Error")));
					break;
				case Enum_SerializationStatus::OPEN_FILE_ERROR:
					statusIterator == 0 ? serializationTypeStatus = "Open File Error" : (statusIterator == 1 ? binarySerializationStatus = "Open File Error" : (statusIterator == 2 ? saveOptionsStatus = "Open File Error" : (statusIterator == 3 ? saveStatus = "Open File Error" : "Error")));
					break;
				case Enum_SerializationStatus::UNSUPPORTED:
					statusIterator == 0 ? serializationTypeStatus = "Unsupported" : (statusIterator == 1 ? binarySerializationStatus = "Unsupported" : (statusIterator == 2 ? saveOptionsStatus = "Unsupported" : (statusIterator == 3 ? saveStatus = "Unsupported" : "Error")));
					break;
				default:
					statusIterator == 0 ? serializationTypeStatus = "Error" : (statusIterator == 1 ? binarySerializationStatus = "Error" : (statusIterator == 2 ? saveOptionsStatus = "Error" : (statusIterator == 3 ? saveStatus = "Error" : "Error")));
					break;
				}
			}
		else
		{
			serializationTypeStatus = "Empty File Path";
		}

	}

	void SaveFileDialog(PWSTR& pwsz)
	{

		IFileDialog* pfd;
		HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
		if (SUCCEEDED(hr)) {
			DWORD dwOptions;
			if (SUCCEEDED(pfd->GetOptions(&dwOptions))) {
				pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
			}
		}

		if (SUCCEEDED(hr)) {
			hr = pfd->Show(NULL);
			if (SUCCEEDED(hr)) {
				IShellItem* psi;
				hr = pfd->GetResult(&psi);
				if (SUCCEEDED(hr)) {
					PWSTR pszPath;
					hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
					if (SUCCEEDED(hr)) {
						pwsz = pszPath;
					}
					psi->Release();
				}
			}
			pfd->Release();
		}

	}

	std::string PWSTRToStdString(PWSTR pwsz) {
		if (pwsz == nullptr) return std::string();

		int len = WideCharToMultiByte(CP_UTF8, 0, pwsz, -1, nullptr, 0, nullptr, nullptr);
		if (len <= 0) return std::string();

		std::vector<char> buffer(len);
	
		WideCharToMultiByte(CP_UTF8, 0, pwsz, -1, &buffer[0], len, nullptr, nullptr);

		return std::string(buffer.begin(), buffer.end() - 1); // Exclude the null terminator
	}

	void DrawSaveOptions(std::string& v_SavePathErrorText, std::string& v_SaveNameErrorText)
	{
		ImGui::Text("Path to save:"); 
		ImGui::SameLine(NULL, 16.0f);

		// Copy the string into the buffer
		ImGui::InputText("##input", s_FileSaveOptions.m_InputBuffer, s_FileSaveOptions.s_BufferSize);
		if (strlen(s_FileSaveOptions.m_InputBuffer) < 3) {
			v_SavePathErrorText = "The path cannot be empty!";
		}
		else
			v_SavePathErrorText.clear();
		ImGui::SameLine();

		if (ImGui::Button("Browse"))
		{
			constexpr int bufferSize = 260;
			char filePath[bufferSize] = "";
			CoInitialize(NULL);
			CoTaskMemFree(s_FileSaveOptions.m_Pwstr);
			SaveFileDialog(s_FileSaveOptions.m_Pwstr);
			strncpy(s_FileSaveOptions.m_InputBuffer, PWSTRToStdString(s_FileSaveOptions.m_Pwstr).c_str(), s_FileSaveOptions.s_BufferSize - 1);
			CoUninitialize();
		}

		ImGui::Text("File Name:");
		ImGui::SameLine(NULL,34.0f);
		
		ImGui::InputText("##input2", s_FileSaveOptions.m_FileNameBuffer, s_FileSaveOptions.s_BufferSize);
		if (strlen(s_FileSaveOptions.m_FileNameBuffer) < 1) {
			v_SaveNameErrorText = "The file name cannot be empty!";
		}
		else
			v_SaveNameErrorText.clear();

		ImGui::Text("Xor Filter:");
		ImGui::SameLine(NULL, 41.0f);
		ImGui::Checkbox("##xorfilter", &s_FileSaveOptions.m_XorFilterCheck);
		if (s_FileSaveOptions.m_XorFilterCheck)
		{
			ImGui::SameLine(NULL, 35.0f);
			ImGui::InputInt("##xorvalue", &s_FileSaveOptions.m_XorValue);
		}

		ImGui::Text("Compression:");
		ImGui::SameLine();
		ImGui::Checkbox("##compress", &s_FileSaveOptions.m_CompressionCheck);

		ImGui::Text("Artifact:");
		ImGui::SameLine(NULL,53.0f);
		ImGui::Checkbox("##artifact", &s_FileSaveOptions.m_ArtifactCheck);
		if (s_FileSaveOptions.m_ArtifactCheck)
		{
			ImGui::SameLine(NULL, 35.0f);
			const char* buffer[3] = { "JSON" , "Binary" , "JSON + Binary" };
			ImGui::Combo("##artifactcombo", &s_FileSaveOptions.m_ArtifactSelected, buffer,3);
		}
	}

	std::string AutoSaveFilenameInit() {
		const time_t now = time(0);
		tm* localTime = localtime(&now);
		int32_t year = localTime->tm_year + 1900;
		int32_t month = localTime->tm_mon + 1;
		int32_t day = localTime->tm_mday;
		int32_t hour = localTime->tm_hour;
		int32_t minute = localTime->tm_min;

		return std::format("AUTOSAVE[{:02}-{:02}-{:4}_{:02}-{:02}].ERA", month, day, year, hour, minute);
	}

	void AutoSaveDirectoryInit() {
		const char* autoSaveDirectoryPath = "C:\\Users\\kocam\\AppData\\Roaming";
		const char* eraPath = "\\ERA";
		std::string fullEraPath = autoSaveDirectoryPath + std::string(eraPath);
		if (fs::exists(fullEraPath))
			std::cout << "File Already Exist!" << std::endl;
		else {
			try
			{
				fs::create_directory(fullEraPath);
			}
			catch (const std::filesystem::filesystem_error& ex)
			{
				std::cerr << "Error creating directory : " << ex.what() << std::endl;
			}
		}
	}

	bool AutoSaveValidator(std::vector<CombineInfo>& savedCombineInfos, std::vector<CombineInfo>& unsavedCombineInfos) {
		if (unsavedCombineInfos.size() == 0)
			return false; // false for new save is NOT required.

		if (savedCombineInfos.size() == 0 || (savedCombineInfos.size() != unsavedCombineInfos.size()))
			return true; // true for new save required.


		for (int32_t combineInfoIterator = 0; combineInfoIterator < savedCombineInfos.size(); combineInfoIterator++) {
			if ((savedCombineInfos[combineInfoIterator].GetTargetItemIdRef() != unsavedCombineInfos[combineInfoIterator].GetTargetItemIdRef()) || (savedCombineInfos[combineInfoIterator].GetCombineCriteriasRef().size() != unsavedCombineInfos[combineInfoIterator].GetCombineCriteriasRef().size()))
				return true;

			std::vector<CombineCriteria>& v_SavedCriterias = savedCombineInfos[combineInfoIterator].GetCombineCriteriasRef();
			std::vector<CombineCriteria>& v_UnsavedCriterias = unsavedCombineInfos[combineInfoIterator].GetCombineCriteriasRef();
			
			for (int32_t criteriaIterator = 0; criteriaIterator < v_SavedCriterias.size(); criteriaIterator++) {
				if ((v_SavedCriterias[criteriaIterator].GetTargetRequirementInfoRef().size() != v_UnsavedCriterias[criteriaIterator].GetTargetRequirementInfoRef().size()) || (v_SavedCriterias[criteriaIterator].GetSourceCriteriasRef().size() != v_UnsavedCriterias[criteriaIterator].GetSourceCriteriasRef().size()))
					return true;
			
				std::vector<RequirementInfo>& v_SavedTargetRequirement = v_SavedCriterias[criteriaIterator].GetTargetRequirementInfoRef();
				std::vector<RequirementInfo>& v_UnsavedTargetRequirement = v_UnsavedCriterias[criteriaIterator].GetTargetRequirementInfoRef();
			
				for (int32_t targetRequirementIterator = 0; targetRequirementIterator < v_SavedTargetRequirement.size(); targetRequirementIterator++) {
					if (v_SavedTargetRequirement[targetRequirementIterator].GetRequirementTypeRef() != v_UnsavedTargetRequirement[targetRequirementIterator].GetRequirementTypeRef())
						return true;
					if (v_SavedTargetRequirement[targetRequirementIterator].GetRequirementValueRef() != v_UnsavedTargetRequirement[targetRequirementIterator].GetRequirementValueRef())
						return true;
				}
				
				std::vector<SourceCriteria>& v_SavedSourceCriterias = v_SavedCriterias[criteriaIterator].GetSourceCriteriasRef();
				std::vector<SourceCriteria>& v_UnsavedSourceCriterias = v_UnsavedCriterias[criteriaIterator].GetSourceCriteriasRef();

				for (int32_t sourceCriteriaIterator = 0; sourceCriteriaIterator < v_SavedSourceCriterias.size(); sourceCriteriaIterator++) {
					if (v_SavedSourceCriterias[sourceCriteriaIterator].GetSourceItemIdRef() != v_UnsavedSourceCriterias[sourceCriteriaIterator].GetSourceItemIdRef())
						return true;
					if (v_SavedSourceCriterias[sourceCriteriaIterator].GetCostInfosRef().size() != v_UnsavedSourceCriterias[sourceCriteriaIterator].GetCostInfosRef().size())
						return true;
					if (v_SavedSourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef().size() != v_UnsavedSourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef().size())
						return true;
					
					std::vector<ProbabilityInfo>& v_SavedProbabilities = v_SavedSourceCriterias[sourceCriteriaIterator].GetProbabilityInfosRef();
					std::vector<ProbabilityInfo>& v_UnsavedProbabilities = v_UnsavedSourceCriterias[sourceCriteriaIterator].GetProbabilityInfosRef();
					for (int32_t probabilityIterator = 0; probabilityIterator < v_SavedProbabilities.size(); probabilityIterator++) {
						if (v_SavedProbabilities[probabilityIterator].GetProbabilityValueRef() != v_UnsavedProbabilities[probabilityIterator].GetProbabilityValueRef())
							return true;
						if (v_UnsavedProbabilities[probabilityIterator].GetProbabiltyTypeRef() != v_UnsavedProbabilities[probabilityIterator].GetProbabiltyTypeRef())
							return true;
					}

					std::vector<RequirementInfo>& v_SavedSourceRequirement = v_SavedSourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef();
					std::vector<RequirementInfo>& v_UnsavedSourceRequirement = v_UnsavedSourceCriterias[sourceCriteriaIterator].GetSourceRequirementInfosRef();
					for (int32_t sourceRequirementIterator = 0; sourceRequirementIterator < v_SavedSourceRequirement.size(); sourceRequirementIterator++) {
						if (v_SavedSourceRequirement[sourceRequirementIterator].GetRequirementTypeRef() != v_UnsavedSourceRequirement[sourceRequirementIterator].GetRequirementTypeRef())
							return true;
						if (v_SavedSourceRequirement[sourceRequirementIterator].GetRequirementValueRef() != v_UnsavedSourceRequirement[sourceRequirementIterator].GetRequirementValueRef())
							return true;
					}

					std::vector<CostInfo>& v_SavedCostInfos = v_SavedSourceCriterias[sourceCriteriaIterator].GetCostInfosRef();
					std::vector<CostInfo>& v_UnsavedCostInfos = v_UnsavedSourceCriterias[sourceCriteriaIterator].GetCostInfosRef();
					for (int32_t costInfoIterator = 0; costInfoIterator < v_SavedCostInfos.size(); costInfoIterator++) {
						if (v_SavedCostInfos[costInfoIterator].GetCostTypeRef() != v_UnsavedCostInfos[costInfoIterator].GetCostTypeRef())
							return true;
						if (v_SavedCostInfos[costInfoIterator].GetCostValueRef() != v_UnsavedCostInfos[costInfoIterator].GetCostValueRef())
							return true;
					}
				}
			}
		}
		return false;
	}

	void AutoSaveOperation(bool& v_AutoSaveCheckBox) {
		int32_t startThreadSec = 5;
		constexpr int32_t autoSaveSeconds = 60;
		std::this_thread::sleep_for(std::chrono::seconds(startThreadSec));
		int32_t saveCounter = 0;
		std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
		std::chrono::time_point endTime = startTime + std::chrono::seconds(autoSaveSeconds);
		while (true) {
			startTime = std::chrono::steady_clock::now();
			if (v_AutoSaveCheckBox && (startTime >= endTime) && (saveCounter > 0)) {
				std::vector<CombineInfo>& unsavedCombineInfos = CombineInfoRoot::Instance().m_CombineInfos;
				std::vector<int32_t> modifiedIndexes;
				for (int32_t validationIterator = 0; validationIterator < unsavedCombineInfos.size(); validationIterator++) {
					if (unsavedCombineInfos[validationIterator].GetCombineModifiedInfoRef()) {
						modifiedIndexes.push_back(validationIterator);
						unsavedCombineInfos[validationIterator].GetCombineModifiedInfoRef() = false;
					}
				}
				if (modifiedIndexes.size() > 0) {
					saveCounter = saveCounter + modifiedIndexes.size();
					AutoSaveDirectoryInit();
					const char* savePath = "C:\\Users\\kocam\\AppData\\Roaming\\ERA";
					strcpy(s_FileAutoSaveOptions.m_InputBuffer, savePath);
					const std::string fileName = AutoSaveFilenameInit();
					strcpy(s_FileAutoSaveOptions.m_FileNameBuffer, fileName.c_str());
					//s_FileAutoSaveOptions.m_XorFilterCheck = true;
					//s_FileAutoSaveOptions.m_XorValue = 122;
					SerializeSpec spec{};
					spec.m_ContentType = Enum_SerizalizeContentType::BINARY;
					spec.m_CombineInfos = &unsavedCombineInfos;
					SerializerManager serializerManager(spec);
					serializerManager.Serialize();
					serializerManager.ProcessForSave(s_FileAutoSaveOptions.TranspileToSaveOptions());
				}
				modifiedIndexes.clear();
				endTime = startTime + std::chrono::seconds(autoSaveSeconds);
			}
			else if ((saveCounter == 0) && v_AutoSaveCheckBox && (startTime >= endTime)) {
				std::vector<int32_t> modifiedIndexes;
				std::vector<CombineInfo>& unsavedCombineInfos = CombineInfoRoot::Instance().m_CombineInfos;
				for (int32_t validationIterator = 0; validationIterator < unsavedCombineInfos.size(); validationIterator++) {
					if (unsavedCombineInfos[validationIterator].GetCombineModifiedInfoRef()) {
						modifiedIndexes.push_back(validationIterator);
						unsavedCombineInfos[validationIterator].GetCombineModifiedInfoRef() = false;
					}
				}
				if (modifiedIndexes.size() > 0) {
					saveCounter = saveCounter + modifiedIndexes.size();
					AutoSaveDirectoryInit();
					const char* savePath = "C:\\Users\\kocam\\AppData\\Roaming\\ERA";
					strcpy(s_FileAutoSaveOptions.m_InputBuffer, savePath);
					const std::string fileName = AutoSaveFilenameInit();
					strcpy(s_FileAutoSaveOptions.m_FileNameBuffer, fileName.c_str());
					//s_FileAutoSaveOptions.m_XorFilterCheck = true;
					//s_FileAutoSaveOptions.m_XorValue = 122;
					SerializeSpec spec{};
					spec.m_ContentType = Enum_SerizalizeContentType::BINARY;
					spec.m_CombineInfos = &unsavedCombineInfos;
					SerializerManager serializerManager(spec);
					serializerManager.Serialize();
					serializerManager.ProcessForSave(s_FileAutoSaveOptions.TranspileToSaveOptions());
					modifiedIndexes.clear();
					endTime = startTime + std::chrono::seconds(autoSaveSeconds);
				}
			}
		}
	}
}