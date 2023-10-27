#pragma once
#include <vector>
#include "CostAndRequirements.h"
class SourceCriteria
{
private:

	uint32_t m_SourceItemId;
	std::vector<CostInfo> m_CostInfos;
	std::vector<RequirementInfo> m_SourceRequirementInfos;

public:

	void SetSourceItemId(uint32_t);
	void SetCostInfo(const CostInfo&);
	void SetSourceRequirementInfo(const RequirementInfo&);
	uint32_t GetSourceItemId() const;
	std::vector<CostInfo> GetCostInfos() const;
	std::vector<RequirementInfo> GetSourceRequirementInfos() const;
};

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