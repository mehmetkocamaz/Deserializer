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
