#pragma once
#include "DataClasses/CombineInfo.h"

class DeserializerManager {
private:
	std::vector<CombineInfo> m_CombineInfos;
public:
	void SetCombineInfos(CombineInfo&);
	std::vector<CombineInfo> GetCombineInfos() const;
	void DisplayScreen() const;
};
