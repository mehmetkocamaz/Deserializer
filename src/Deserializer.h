#pragma once

#include <cstdint>
#include <vector>

enum class Enum_Requirement
{
	Enchanment = 0,
	Combine,
	Refine,
	Socket
};

enum class Enum_Cost
{
	Silver = 0,
	Billion,
	ContributionPoint,
	BloodPoint
};

struct CostInfo
{
	Enum_Cost m_CostType;
	int32_t m_CostValue;
};

struct RequirementInfo
{
	Enum_Requirement m_RequirementType;
	int32_t m_RequirementValue;
};

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



