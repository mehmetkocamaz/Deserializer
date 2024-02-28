#pragma once
#include "DataClasses/CombineInfo.h"
#include "DeserializeSpec.h"
#include <vector>
#include <bitset>

enum class Enum_DeserializationStatus
{
	SUCCESS = 0,
	FAIL,
	UNSUPPORTED,
	OPEN_FILE_ERROR,
	XOR_KEY_ERROR,
	XOR_CHECK_ERROR,
	DECOMPRESSION_ERROR,
	DECOMPRESSION_CHECK_ERROR,
	EMPTY_BUFFER
	// MORE ERROR TYPES..
};

class DeserializerManager {

public:
	DeserializerManager(DeserializeSpec DeserializeSpecification) 
		: m_DeserializeSpecification(DeserializeSpecification) { }

#pragma region Setters
	void PushCombineInfos(CombineInfo& obj) { m_CombineInfos.push_back(obj); }
#pragma endregion
#pragma region Getters
	std::vector<CombineInfo> GetCombineInfos() const;
	std::filesystem::path GetFilePath()const { return m_FilePath; }
	std::vector<Enum_DeserializationStatus>& GetDeserializationStatusRef() { return m_Status; }
	void FlagValidator();
#pragma endregion

	Enum_DeserializationStatus Deserialize();
	void DisplayScreen() const;

private:
	std::vector<CombineInfo> m_CombineInfos;
	std::filesystem::path m_FilePath;
	//Enum_DeserializationStatus JsonDeserialize(m_FilePath);
	Enum_DeserializationStatus JsonDeserialize();
	Enum_DeserializationStatus BinaryDeserialize();
	DeserializeSpec m_DeserializeSpecification;
	std::vector<Enum_DeserializationStatus> m_Status;
	int8_t m_LoadFlags;
};

