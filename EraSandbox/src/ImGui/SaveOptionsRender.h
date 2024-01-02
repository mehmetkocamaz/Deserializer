#pragma once
#include "imgui.h"
#include <iostream>
#include <windows.h>
#include <shobjidl.h> 
#include <cstring>


#include <sys/stat.h>
#include <direct.h>
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

	void AutoSaveFilenameInit(time_t& v_Now, std::string& v_FileName) {
		tm* localTime = localtime(&v_Now);
		int32_t year = localTime->tm_year + 1900;
		int32_t month = localTime->tm_mon + 1;
		int32_t day = localTime->tm_mday;
		int32_t hour = localTime->tm_hour;
		int32_t minute = localTime->tm_min;
		v_FileName = "AUTOSAVE[" + std::to_string(month) + "/" + std::to_string(day) + "/" + std::to_string(year) + "/" + std::to_string(hour) + " : " + std::to_string(minute) + "]";
	}

	void AutoSaveDirectoryInit() {
		const char* autoSaveDirectoryPath = "C:\\Users\\kocam\\AppData\\Roaming";
		const char* eraPath = "\\ERA";
		std::string fullEraPath = autoSaveDirectoryPath + std::string(eraPath);
		int result = _mkdir(fullEraPath.c_str());

		if (result == 0) {
			//ImGui::Text("Directory created on %s", fullEraPath.c_str());
		}
		//else
			//ImGui::Text("Directory cannot created!");
	}

	void AutoSaveOperation(std::vector<CombineInfo>& v_CombineInfos, std::chrono::time_point < std::chrono::steady_clock>& s_StartTime, std::chrono::time_point < std::chrono::steady_clock>& s_EndTime, bool& s_AutoSaveThreadRunning) {

		AutoSaveDirectoryInit();
		if (s_StartTime == s_EndTime) {
			s_AutoSaveThreadRunning = true;
			const char* savePath = "C:\\Users\\kocam\\AppData\\Roaming\\ERA";
			strcpy(s_FileAutoSaveOptions.m_InputBuffer, savePath);
			time_t now = time(0);
			std::string fileName;
			AutoSaveFilenameInit(now, fileName);
			s_FileAutoSaveOptions.m_XorValue = 22012001;
			strcpy(s_FileAutoSaveOptions.m_FileNameBuffer, fileName.c_str());

			SerializeSpec spec{};
			spec.m_ContentType = Enum_SerizalizeContentType::BINARY;
			spec.m_CombineInfos = &v_CombineInfos;
			SerializerManager serializerManager(spec);
			serializerManager.Serialize();
			serializerManager.ProcessForSave(s_FileAutoSaveOptions.TranspileToSaveOptions());
			s_StartTime = std::chrono::steady_clock::now();
			s_EndTime = s_StartTime + std::chrono::minutes(2);
		}
		else
			s_AutoSaveThreadRunning = false;
	}


	//void DrawAutoSave() {
	//	
	//}


}