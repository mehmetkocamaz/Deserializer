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