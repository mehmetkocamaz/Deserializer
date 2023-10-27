#pragma once
#include <bitset>
#include "DeserializeManager.h"
class BinaryManager {
public:
	void SetBinaryData(const DeserializerManager&);
	std::vector<std::bitset<32>> GetBinaryData() const;
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

