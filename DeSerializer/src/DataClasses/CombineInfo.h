#pragma once
#include <vector>
#include <cstdint>
#include "CombineCriteria.h"

class CombineInfo
{
public:
	void SetTargetItemId(uint32_t);
	void SetCombineCriterias(CombineCriteria&);
	uint32_t GetTargetItemId() const;
	uint32_t& GetTargetItemIdRef()
	{
		return m_TargetItemId;
	}
	std::vector<CombineCriteria> GetCombineCriterias() const;
	std::vector<CombineCriteria>& GetCombineCriteriasRef()
	{
		return m_CombineCriterias;
	}
private:
	uint32_t m_TargetItemId = 0; // Always default initialize
	std::vector<CombineCriteria> m_CombineCriterias;
};




//vector<CombineInfo> 0. 
//
//4 byte
//
//00 00 00 00 
//
//
//00 00 00 00 
//
//00 00 00 00
//
