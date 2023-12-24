#pragma once
#include "DataClasses/CombineInfo.h"
#include "DeserializeSpec.h"
#include <vector>
#include <bitset>

enum class Enum_DeserializationStatus
{
	SUCCESS = 0,
	FAIL = 1,
	UNSUPPORTED = 2,
	OPEN_FILE_ERROR = 3
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
#pragma endregion

	Enum_DeserializationStatus Deserialize();
	//void DisplayScreen() const;

private:
	std::vector<CombineInfo> m_CombineInfos;
	std::filesystem::path m_FilePath;
	//Enum_DeserializationStatus JsonDeserialize(m_FilePath);
	Enum_DeserializationStatus JsonDeserialize();
	Enum_DeserializationStatus BinaryDeserialize();
	DeserializeSpec m_DeserializeSpecification;
};

