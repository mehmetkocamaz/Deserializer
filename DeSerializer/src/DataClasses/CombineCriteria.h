#pragma once
#include "SourceCriteria.h"

class CombineCriteria
{

public:
	void SetSourceCriterias(SourceCriteria&);
	void SetTargetRequirementInfo(RequirementInfo&);
	std::vector<SourceCriteria> GetSourceCriterias() const;
	std::vector<SourceCriteria>& GetSourceCriteriasRef() {
		return m_SourceCriterias;
	}
	std::vector<RequirementInfo> GetTargetRequirementInfo() const;

	std::vector<RequirementInfo>& GetTargetRequirementInfoRef()
	{
		return m_TargetRequirementInfos;
	}

private:
	std::vector<RequirementInfo> m_TargetRequirementInfos;
	std::vector<SourceCriteria> m_SourceCriterias;

};
