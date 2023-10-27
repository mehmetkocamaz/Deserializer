#include "CombineInfo.h"

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
