#pragma once
#include <iostream>
#include <fstream>
#include "SerializeManager.h"
#include "BinaryManager/BinaryManager.h"

Enum_SerializationStatus SerializerManager::Serialize() {
	switch (m_SerializeSpecification.m_FileType)
	{
	case Enum_SerizalizeContentType::JSON:
		return JsonSerialize();
	case Enum_SerizalizeContentType::BINARY:
		return BinarySerialize();
	default:
		return Enum_SerializationStatus::UNSUPPORTED;
	}
}

Enum_SerializationStatus SerializerManager::JsonSerialize() {
	return Enum_SerializationStatus::SUCCESS;
}

Enum_SerializationStatus SerializerManager::BinarySerialize() {

	m_BinaryData.push_back(m_SerializeSpecification.m_CombineInfos->size());
	for (const auto& combineInfo : *m_SerializeSpecification.m_CombineInfos) {
		m_BinaryData.push_back(combineInfo.GetTargetItemId());
		m_BinaryData.push_back(combineInfo.GetCombineCriterias().size());
		for (const auto& combineCriterias : combineInfo.GetCombineCriterias()) {
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
				m_BinaryData.push_back(sourceCriterias.GetProbabilityInfos().size());
				for (const auto& probabilityInfo : sourceCriterias.GetProbabilityInfos())
				{
					m_BinaryData.push_back(static_cast<int32_t>(probabilityInfo.m_ProbabilityType));
					m_BinaryData.push_back(probabilityInfo.m_ProbabilityValue);
				}
				m_BinaryData.push_back(sourceCriterias.GetSourceRequirementInfos().size());
				for (const auto& sourceRequirementInfo : sourceCriterias.GetSourceRequirementInfos()) {
					m_BinaryData.push_back(static_cast<int32_t>(sourceRequirementInfo.m_RequirementType));
					m_BinaryData.push_back(sourceRequirementInfo.m_RequirementValue);
				}
			}
		}
	}


	std::ofstream binaryFile(m_SerializeSpecification.m_FilePath, std::ios::binary);

	if (!binaryFile.is_open())
	{
		std::cerr << "data.bin cannot opened!!" << std::endl;
		return Enum_SerializationStatus::OPEN_FILE_ERROR;
	}
	try
	{
		for (const std::bitset<32> data : m_BinaryData)
			binaryFile.write(reinterpret_cast<const char*>(&data), sizeof(data));
		
		binaryFile.close();
	}
	catch (const std::exception&)
	{
		return Enum_SerializationStatus::FAIL;
	}

	return Enum_SerializationStatus::SUCCESS;
}