#pragma once
#include <bitset>
#include "DeserializeManager/DeserializeManager.h"

//class HierarchyController {
//public: 
//	void SetDeserializeManagerData(const DeserializerManager);
//	DeserializerManager GetDeserializedData();
//private:
//	DeserializerManager m_DeseriaizerManager;
//};

class BinaryManager {
public:
	void SetBinaryData(const DeserializerManager&);
	std::vector<std::bitset<32>> GetBinaryData() const;
	//void SaveToFile();
	//void FetchFromFile();
private:
	std::vector<std::bitset<32>> m_BinaryData;
};