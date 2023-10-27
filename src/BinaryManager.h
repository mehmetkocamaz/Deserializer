#pragma once
#include <bitset>
#include "DeserializeManager.h"
class BinaryManager {
public:
	void SetBinaryData(const DeserializerManager&);
	std::vector<std::bitset<32>> GetBinaryData() const;
	void SaveToFile();
	void FetchFromFile();
private:
	std::vector<std::bitset<32>> m_BinaryData;
};

void BinaryManager::SetBinaryData(const DeserializerManager& obj) {

	m_BinaryData.push_back(obj.GetCombineInfos().size());
	for (const auto& combineInfos : obj.GetCombineInfos()) {
		m_BinaryData.push_back(combineInfos.GetTargetItemId());
		m_BinaryData.push_back(combineInfos.GetCombineCriterias().size());
		for (const auto& combineCriterias : combineInfos.GetCombineCriterias()) {
			m_BinaryData.push_back(combineCriterias.GetTargetRequirementInfo().size());
			for (const auto& targetRequirementInfo : combineCriterias.GetTargetRequirementInfo()) {
				m_BinaryData.push_back(static_cast<int32_t>(targetRequirementInfo.m_RequirementType));
				m_BinaryData.push_back(targetRequirementInfo.m_RequirementValue);
			}
			m_BinaryData.push_back(combineCriterias.GetSourceCriterias().size());
			for (const auto& sourceCriterias : combineCriterias.GetSourceCriterias()) {
				m_BinaryData.push_back(sourceCriterias.GetSourceItemId());
				m_BinaryData.push_back(sourceCriterias.GetCostInfos().size());
				for (const auto& sourceCostInfo : sourceCriterias.GetCostInfos()) {
					m_BinaryData.push_back(static_cast<int32_t>(sourceCostInfo.m_CostType));
					m_BinaryData.push_back(sourceCostInfo.m_CostValue);
				}
				m_BinaryData.push_back(sourceCriterias.GetSourceRequirementInfos().size());
				for (const auto& sourceRequirementInfo : sourceCriterias.GetSourceRequirementInfos()) {
					m_BinaryData.push_back(static_cast<int32_t>(sourceRequirementInfo.m_RequirementType));
					m_BinaryData.push_back(sourceRequirementInfo.m_RequirementValue);
				}
			}
		}
	}
}

std::vector<std::bitset<32>> BinaryManager::GetBinaryData() const {
	return m_BinaryData;
}

void BinaryManager::SaveToFile() {
	BinaryManager binaryData;
	DeserializerManager deserializerManager;
	deserializerManager.Deserialize();
	binaryData.SetBinaryData(deserializerManager);

	std::ofstream binaryFile("data.bin", std::ios::binary);
	if (binaryFile.is_open()) {
		for (const std::bitset<32>data : binaryData.GetBinaryData()) {
			binaryFile.write(reinterpret_cast<const char*>(&data), sizeof(data));
		}
		binaryFile.close();
	}
	else {
		std::cerr << "data.bin cannot opened!!" << std::endl;
	}
}

void BinaryManager::FetchFromFile() {
	std::ifstream readData("data.bin", std::ios::binary);
	if (readData.is_open()) {
		std::vector<std::bitset<32>> binaryData;

		while (!readData.eof()) {
			std::bitset<32> temp;
			readData.read(reinterpret_cast<char*>(&temp), sizeof(temp));
			binaryData.push_back(temp);
		}

		readData.close();

		for (const auto& item : binaryData) {
			std::cout << item << std::endl;
		}
	}
	else {
		std::cerr << "data.bin cannot opened!!" << std::endl;
	}
}