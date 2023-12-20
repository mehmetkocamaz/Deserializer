#pragma once
#include "SourceCriteria.h"

class CombineCriteria
{
public:
#pragma region Setters
	void PushSourceCriterias(const SourceCriteria& p_SourceCriteria) { m_SourceCriterias.push_back(p_SourceCriteria); }
	void PushTargetRequirementInfo(const RequirementInfo& p_TargetRequirementInfo) { m_TargetRequirementInfos.push_back(p_TargetRequirementInfo); }
#pragma endregion
#pragma region Getters
	std::vector<SourceCriteria> GetSourceCriterias() const { return m_SourceCriterias; }
	std::vector<SourceCriteria>& GetSourceCriteriasRef() { return m_SourceCriterias; }
	std::vector<RequirementInfo> GetTargetRequirementInfo() const { return m_TargetRequirementInfos; }
	std::vector<RequirementInfo>& GetTargetRequirementInfoRef() { return m_TargetRequirementInfos; }
	bool& GetCombineCriteriaStatus() { return m_Status; }
#pragma endregion
private:
	std::vector<RequirementInfo> m_TargetRequirementInfos;
	std::vector<SourceCriteria> m_SourceCriterias;
	bool m_Status = false;
};
