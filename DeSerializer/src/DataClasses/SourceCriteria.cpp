#include "SourceCriteria.h"

// SOURCE CRITERIA FUNCTION DECLARATIONS
// ~Setters
void SourceCriteria::SetSourceItemId(uint32_t itemId) {
	m_SourceItemId = itemId;
}
void SourceCriteria::SetCostInfo(const CostInfo& obj) {
	m_CostInfos.push_back(obj);
}
void SourceCriteria::SetSourceRequirementInfo(const RequirementInfo& obj) {
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