#pragma once
#include "DataClasses/CombineInfo.h"
#include "DeserializeSpec.h"
#include <vector>

enum class Enum_DeserializationStatus
{
	SUCCESS = 0,
	FAIL = 1,
	UNSUPPORTED = 2,
	OPEN_FILE_ERROR = 3
	// MORE ERROR TYPES..
};

class DeserializerManager {
private:
	std::vector<CombineInfo> m_CombineInfos;
	std::filesystem::path m_FilePath;
	//Enum_DeserializationStatus JsonDeserialize(m_FilePath);
	Enum_DeserializationStatus JsonDeserialize(std::filesystem::path filePath);
	Enum_DeserializationStatus BinaryDeserialize(std::filesystem::path filePath);
public:
	//DeserializerManager(std::filesystem::path filePath) 
	//	: m_FilePath(filePath) { }
	void SetCombineInfos(CombineInfo&);
	std::vector<CombineInfo> GetCombineInfos() const;
	Enum_DeserializationStatus Deserialize(const DeserializeSpec& deserializeSpec);
	void DisplayScreen() const;
};

