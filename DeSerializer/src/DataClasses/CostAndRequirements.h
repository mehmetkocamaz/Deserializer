#pragma once
#include <cstdint>
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

enum class Enum_Probability
{
	Success = 0,
	Fail = 1,
	Break = 2
};

struct CostInfo
{

	Enum_Cost m_CostType = Enum_Cost::Silver;
	uint32_t m_CostValue = 0;
	Enum_Cost& GetCostTypeRef() { return m_CostType; }
	uint32_t& GetCostValueRef() { return m_CostValue; }

};

struct RequirementInfo
{

	Enum_Requirement m_RequirementType = Enum_Requirement::Enchanment;
	uint32_t m_RequirementValue = 0;
	Enum_Requirement& GetRequirementTypeRef() { return m_RequirementType; }
	uint32_t& GetRequirementValueRef() { return m_RequirementValue; }
};

struct ProbabilityInfo
{
	Enum_Probability m_ProbabilityType = Enum_Probability::Success;
	uint32_t m_ProbabilityValue = 0;
};