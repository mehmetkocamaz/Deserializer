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

