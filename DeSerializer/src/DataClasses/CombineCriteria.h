#pragma once
#include "SourceCriteria.h"

class CombineCriteria
{
public:
#pragma region Setters
	void PushSourceCriterias(SourceCriteria& obj) { m_SourceCriterias.push_back(obj); }
	void PushTargetRequirementInfo(RequirementInfo& obj) { m_TargetRequirementInfos.push_back(obj); }
#pragma endregion
#pragma region Getters
	std::vector<SourceCriteria> GetSourceCriterias() const { return m_SourceCriterias; }
	std::vector<SourceCriteria>& GetSourceCriteriasRef() { return m_SourceCriterias; }
	std::vector<RequirementInfo> GetTargetRequirementInfo() const { return m_TargetRequirementInfos; }
	std::vector<RequirementInfo>& GetTargetRequirementInfoRef() { return m_TargetRequirementInfos; }
#pragma endregion
private:
	std::vector<RequirementInfo> m_TargetRequirementInfos;
	std::vector<SourceCriteria> m_SourceCriterias;
};
