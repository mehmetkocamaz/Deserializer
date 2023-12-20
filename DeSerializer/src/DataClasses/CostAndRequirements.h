#pragma once
#include <cstdint>
enum class Enum_Requirement
{
	None = 0,
	Enchanment,
	Combine,
	Refine,
	Socket
};

enum class Enum_Cost
{
	None = 0,
	Silver,
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

	Enum_Cost m_CostType = Enum_Cost::None;
	uint32_t m_CostValue = 0;
#pragma region Getters
	Enum_Cost& GetCostTypeRef() { return m_CostType; }
	uint32_t& GetCostValueRef() { return m_CostValue; }
#pragma endregion
};

struct RequirementInfo
{
	Enum_Requirement m_RequirementType = Enum_Requirement::None;
	uint32_t m_RequirementValue = 0;
#pragma region Getters
	Enum_Requirement& GetRequirementTypeRef() { return m_RequirementType; }
	uint32_t& GetRequirementValueRef() { return m_RequirementValue; }
#pragma endregion
};

struct ProbabilityInfo
{
	Enum_Probability m_ProbabilityType = Enum_Probability::Success;
	uint32_t m_ProbabilityValue = 0;
#pragma region Getters
	Enum_Probability& GetProbabiltyTypeRef() { return m_ProbabilityType; }
	uint32_t& GetProbabilityValueRef() { return m_ProbabilityValue; }
#pragma endregion
};