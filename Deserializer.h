#pragma once
#ifndef DESERIALIZER_H
#define DESERIALIZER_H
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
	void SetSourceId(uint32_t itemId) {
		m_SourceItemId = itemId;
	}
	uint32_t GetSourceId()const  {
		return m_SourceItemId;
	}
};

class CombineCriteria
{
private:
	std::vector<RequirementInfo> m_TargetRequirementInfos;
	std::vector<SourceCriteria> m_SourceCriterias;
};

class CombineInfo
{
private:
	uint32_t m_TargetItemId;
	std::vector<CombineCriteria> m_CombineCriterias;
public:
	void SetTargetId(uint32_t id) {
		m_TargetItemId = id;
	}
	uint32_t GetTargetId() const {
		return m_TargetItemId;
	}
	
};

#endif // !DESERIALIZER_H
