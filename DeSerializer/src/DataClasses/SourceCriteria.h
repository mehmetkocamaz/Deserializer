#pragma once
#include <vector>
#include "CostAndRequirements.h"
class SourceCriteria
{
public:
#pragma region Setters
	void SetSourceItemId(uint32_t sourceItemId)
	{
		m_SourceItemId = sourceItemId;
	}

	void SetCostInfo(const CostInfo& costInfo)
	{
		m_CostInfos.push_back(costInfo);
	}

	void SetSourceRequirementInfo(const RequirementInfo& requirementInfo)
	{
		m_SourceRequirementInfos.push_back(requirementInfo);
	}
#pragma endregion 
#pragma region Getters
	uint32_t GetSourceItemId() const
	{
		return m_SourceItemId;
	}

	uint32_t& GetSourceItemIdRef()
	{
		return m_SourceItemId;
	}

	std::vector<CostInfo> GetCostInfos() const
	{
		return m_CostInfos;
	}

	std::vector<CostInfo>& GetCostInfosRef()
	{
		return m_CostInfos;
	}

	std::vector<RequirementInfo> GetSourceRequirementInfos() const
	{
		return m_SourceRequirementInfos;
	}

	std::vector<RequirementInfo>& GetSourceRequirementInfosRef() 
	{
		return m_SourceRequirementInfos;
	}

	std::vector<ProbabilityInfo> GetProbabilityInfos() const
	{
		return m_ProbabilityInfos;
	}

	std::vector<ProbabilityInfo>& GetProbabilityInfosRef()
	{
		return m_ProbabilityInfos;
	}
#pragma endregion
private:

	uint32_t m_SourceItemId = 0;
	std::vector<CostInfo> m_CostInfos;
	std::vector<ProbabilityInfo> m_ProbabilityInfos;
	std::vector<RequirementInfo> m_SourceRequirementInfos;
};

