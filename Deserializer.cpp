#include "Deserializer.h"
#include <iostream>
// SOURCE CRITERIA FUNCTION DECLARATIONS
// ~Setters
void SourceCriteria::SetSourceItemId(uint32_t itemId) {
	m_SourceItemId = itemId;
}
void SourceCriteria::SetCostInfo(CostInfo& obj) {
	m_CostInfos.push_back(obj);
}
void SourceCriteria::SetSourceRequirementInfo(RequirementInfo& obj) {
	m_SourceRequirementInfos.push_back(obj);
}
// ~Getters
uint32_t SourceCriteria::GetSourceItemId()const {

	return m_SourceItemId;
}
std::vector<CostInfo> SourceCriteria::GetCostInfos() const {
	return m_CostInfos;
}
std::vector<RequirementInfo> SourceCriteria::GetSourceRequirementInfos() const {
	return m_SourceRequirementInfos;
}


// COMBINE CRITERIA FUNCTION DECLARATIONS
// ~Setters
void CombineCriteria::SetSourceCriterias(SourceCriteria& obj) {
	m_SourceCriterias.push_back(obj);
}
void CombineCriteria::SetTargetRequirementInfo(RequirementInfo& obj) {
	m_TargetRequirementInfos.push_back(obj);
}
// ~Getters
std::vector<RequirementInfo> CombineCriteria::GetTargetRequirementInfo() const {
	return m_TargetRequirementInfos;
}
std::vector<SourceCriteria> CombineCriteria::GetSourceCriterias() const {
	return m_SourceCriterias;
}


// COMBINE INFO FUNCTION DECLARATIONS
// ~Setters
void CombineInfo::SetTargetItemId(uint32_t id) {
	m_TargetItemId = id;
}
void CombineInfo::SetCombineCriterias(CombineCriteria& obj) {
	m_CombineCriterias.push_back(obj);
}
// ~Getters
uint32_t CombineInfo::GetTargetItemId() const {
	return m_TargetItemId;
}
std::vector<CombineCriteria> CombineInfo::GetCombineCriterias() const {
	return m_CombineCriterias;
}


// DESERIALIZER MANAGER FUNCTION DECLARATIONS
void DeserializerManager::DisplayScreen() const {
	for (size_t i = 0; i < m_CombineInfos.size(); i++) // LOOP FOR COMBINE INFO VECTOR
	{
		std::cout << "Combine Info: \n" << "Target Item ID: " << m_CombineInfos.at(i).GetTargetItemId() << std::endl;
		std::cout << "Combine Criterias: " << std::endl;
		for (size_t j = 0; j < m_CombineInfos.at(i).GetCombineCriterias().size(); j++) // LOOP FOR COMBINE CRITERIA VECTOR
		{
			std::cout << "Target Requirement Infos: " << std::endl;
			for (size_t k = 0; k < m_CombineInfos.at(i).GetCombineCriterias().at(j).GetTargetRequirementInfo().size(); k++) // LOOP FOR TARGET REQUIREMENT INFOS
			{
				std::cout << "Target Requirement Type: " << static_cast<int>(m_CombineInfos.at(i).GetCombineCriterias().at(j).GetTargetRequirementInfo().at(k).m_RequirementType) << std::endl;
				std::cout << "Target Requirement Value: " << m_CombineInfos.at(i).GetCombineCriterias().at(j).GetTargetRequirementInfo().at(k).m_RequirementValue << std::endl;
			}
			std::cout << "Source Criterias: " << std::endl;
			for (size_t k = 0; k < m_CombineInfos.at(i).GetCombineCriterias().at(j).GetSourceCriterias().size(); k++) // LOOP FOR SOURCE CRITERIAS
			{
				std::cout << "Source Item ID: " << m_CombineInfos.at(i).GetCombineCriterias().at(j).GetSourceCriterias().at(k).GetSourceItemId() << std::endl;
				std::cout << "Source Cost Infos: " << std::endl;
				for (size_t l = 0; l < m_CombineInfos.at(i).GetCombineCriterias().at(j).GetSourceCriterias().at(k).GetCostInfos().size(); l++) // LOOP FOR SOURCE COST INFOS
				{
					std::cout << "Cost Type: " << static_cast<int>(m_CombineInfos.at(i).GetCombineCriterias().at(j).GetSourceCriterias().at(k).GetCostInfos().at(l).m_CostType) << std::endl;
					std::cout << "Cost Value" << m_CombineInfos.at(i).GetCombineCriterias().at(j).GetSourceCriterias().at(k).GetCostInfos().at(l).m_CostValue << std::endl;
				}
				std::cout << "Source Requirement Infos: " << std::endl;
				for (size_t l = 0; l < m_CombineInfos.at(i).GetCombineCriterias().at(j).GetSourceCriterias().at(k).GetSourceRequirementInfos().size(); l++) // LOOP FOR SOURCE REQUIREMENT INFOS
				{
					std::cout << "Requirement Type: " << static_cast<int>(m_CombineInfos.at(i).GetCombineCriterias().at(j).GetSourceCriterias().at(k).GetSourceRequirementInfos().at(l).m_RequirementType) << std::endl;
					std::cout << "Requirement Value: " << m_CombineInfos.at(i).GetCombineCriterias().at(j).GetSourceCriterias().at(k).GetSourceRequirementInfos().at(l).m_RequirementValue << std::endl;
				}
			}
		}
	}
}
// ~Setters
void DeserializerManager::SetCombineInfos(CombineInfo& obj) {
	m_CombineInfos.push_back(obj);
}
// ~Getters
std::vector<CombineInfo> DeserializerManager::GetCombineInfos() const {
	return m_CombineInfos;
}
