#pragma once
#include <bitset>
#include "DeserializeManager/DeserializeManager.h"

class BinaryManager {
public:
	std::vector<std::bitset<32>> GetBinaryData() const;
private:
	std::vector<std::bitset<32>> m_BinaryData;
};