#pragma once
#include <bitset>
#include "DeserializeManager/DeserializeManager.h"

class BinaryManager {
public:
	void SetBinaryData(const DeserializerManager&);
	std::vector<std::bitset<32>> GetBinaryData() const;
	void SaveToFile();
	void FetchFromFile();
private:
	std::vector<std::bitset<32>> m_BinaryData;
};