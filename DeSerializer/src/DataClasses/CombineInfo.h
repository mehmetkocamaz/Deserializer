#pragma once
#include <vector>
#include <cstdint>
#include "CombineCriteria.h"

class CombineInfo
{
public:
#pragma region Setters
	void SetTargetItemId(uint32_t id) { m_TargetItemId = id; }
	void PushCombineCriterias(const CombineCriteria& p_CombineCriteria) { m_CombineCriterias.push_back(p_CombineCriteria); }

#pragma region Getters
	uint32_t GetTargetItemId() const { return m_TargetItemId; }
	uint32_t& GetTargetItemIdRef() { return m_TargetItemId; }
	std::vector<CombineCriteria> GetCombineCriterias() const { return m_CombineCriterias; }
	std::vector<CombineCriteria>& GetCombineCriteriasRef() { return m_CombineCriterias; }
#pragma endregion
private:
	uint32_t m_TargetItemId = 0; // Always default initialize
	std::vector<CombineCriteria> m_CombineCriterias;
};

