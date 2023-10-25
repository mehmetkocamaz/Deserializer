#pragma once
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "Deserializer.h"

void CostInfoFiller(CostInfo& obj, int type, int32_t value) {
	obj.m_CostType = static_cast<Enum_Cost>(type);
	obj.m_CostValue = value;
}

void RequirementInfoFiller(RequirementInfo& obj, int type, int32_t value) {
	obj.m_RequirementType = static_cast<Enum_Requirement>(type);
	obj.m_RequirementValue = value;
}

//struct RequirementInfo
//{
//	Enum_Requirement m_RequirementType;
//	int32_t m_RequirementValue;
//};

#endif // !FUNCTIONS_H
