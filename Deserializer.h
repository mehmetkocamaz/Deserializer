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
	void SetSourceItemId(uint32_t itemId) {
		m_SourceItemId = itemId;
	}
	uint32_t GetSourceItemId()const  {
		return m_SourceItemId;
	}
	void SetCostInfo(CostInfo& obj) {
		m_CostInfos.push_back(obj);
	}
	void SetSourceRequirementInfo(RequirementInfo& obj) {
		m_SourceRequirementInfos.push_back(obj);
	}
};

class CombineCriteria
{
private:
	std::vector<RequirementInfo> m_TargetRequirementInfos;
	std::vector<SourceCriteria> m_SourceCriterias;

public:
	void SetSourceCriterias(SourceCriteria& obj) {
		m_SourceCriterias.push_back(obj);
	}
	void SetTargetRequirementInfo(RequirementInfo& obj) {
		m_TargetRequirementInfos.push_back(obj);
	}
};

class CombineInfo
{
private:
	uint32_t m_TargetItemId;
	std::vector<CombineCriteria> m_CombineCriterias;

public:
	bool SetTargetItemId(uint32_t id) {
		if (m_TargetItemId = id) {
			return true;
		}
		return false;
	}
	uint32_t GetTargetItemId() const {
		return m_TargetItemId;
	}
	
	void SetCombineCriterias(CombineCriteria& obj) {
		m_CombineCriterias.push_back(obj);
	}

};

#endif // !DESERIALIZER_H
