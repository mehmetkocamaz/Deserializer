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
	std::vector<CombineCriteria> GetCombineCriterias() const;

private:
	uint32_t m_TargetItemId;
	std::vector<CombineCriteria> m_CombineCriterias;
};

// COMBINE INFO FUNCTION DECLARATIONS
// ~Setters
void CombineInfo::SetTargetItemId(uint32_t id) {
	m_TargetItemId = id;
}
void CombineInfo::SetCombineCriterias(CombineCriteria& obj) {
	m_CombineCriterias.push_back(obj);
}
// ~Getters
uint32_t CombineInfo::GetTargetItemId() const {
	return m_TargetItemId;
}
std::vector<CombineCriteria> CombineInfo::GetCombineCriterias() const {
	return m_CombineCriterias;
}



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
