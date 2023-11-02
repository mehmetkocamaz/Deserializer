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

	void SetCombineInfos(CombineInfo&);
	std::vector<CombineInfo> GetCombineInfos() const;
	Enum_DeserializationStatus Deserialize();
	void DisplayScreen() const;

	void ReadUint32_t(const std::vector<std::bitset<32>>&, uint32_t&, uint32_t&, uint32_t&);
	void ReadInt32_t(const std::vector<std::bitset<32>>&, int32_t&, uint32_t&, uint32_t&);
	void ReadEnum_Cost(const std::vector<std::bitset<32>>&, Enum_Cost&, uint32_t&, uint32_t&);
	void ReadEnum_Requirement(const std::vector<std::bitset<32>>&, Enum_Requirement&, uint32_t&, uint32_t&);
	void ReadEnum_Probability(const std::vector<std::bitset<32>>&, Enum_Probability&, uint32_t&, uint32_t&);
private:
	std::vector<CombineInfo> m_CombineInfos;
	std::filesystem::path m_FilePath;
	//Enum_DeserializationStatus JsonDeserialize(m_FilePath);
	Enum_DeserializationStatus JsonDeserialize();
	Enum_DeserializationStatus BinaryDeserialize();
	DeserializeSpec m_DeserializeSpecification;
};

