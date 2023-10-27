#pragma once
#include "SourceCriteria.h"
class CombineCriteria
{
private:
	std::vector<RequirementInfo> m_TargetRequirementInfos;
	std::vector<SourceCriteria> m_SourceCriterias;

public:
	void SetSourceCriterias(SourceCriteria&);
	void SetTargetRequirementInfo(RequirementInfo&);
	std::vector<SourceCriteria> GetSourceCriterias() const;
	std::vector<RequirementInfo> GetTargetRequirementInfo() const;
};

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