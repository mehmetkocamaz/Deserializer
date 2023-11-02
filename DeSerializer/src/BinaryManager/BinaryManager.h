#pragma once
#include <bitset>
#include "DeserializeManager/DeserializeManager.h"
class BinaryManager {
public:
#pragma region Getters
	std::vector<std::bitset<32>> GetBinaryData() const { return m_BinaryData; };
#pragma endregion
private:
	std::vector<std::bitset<32>> m_BinaryData;
};