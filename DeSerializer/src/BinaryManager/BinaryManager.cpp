#include "BinaryManager.h"
#include <fstream>
#include <iostream>

void BinaryManager::SetBinaryData(const DeserializerManager& obj) {


}


std::vector<std::bitset<32>> BinaryManager::GetBinaryData() const {
	return m_BinaryData;
}

//void BinaryManager::SaveToFile() {
//	BinaryManager binaryData;
//	DeserializerManager deserializerManager;
//	Enum_DeserializationStatus deserializationStatus =
//		deserializerManager.Deserialize(DeserializeSpec{
//			.m_FileType = Enum_ContentType::JSON, .m_FilePath = "resources/target.json" });
//	binaryData.SetBinaryData(deserializerManager);
//
//	std::ofstream binaryFile("output/data.bin", std::ios::binary);
//	if (binaryFile.is_open()) {
//		for (const std::bitset<32>data : binaryData.GetBinaryData()) {
//			binaryFile.write(reinterpret_cast<const char*>(&data), sizeof(data));
//		}
//		binaryFile.close();
//	}
//	else {
//		std::cerr << "data.bin cannot opened!!" << std::endl;
//	}
//}
//
//void BinaryManager::FetchFromFile() {
//	std::ifstream readData("output/data.bin", std::ios::binary);
//	if (readData.is_open()) {
//		std::vector<std::bitset<32>> binaryData;
//
//		while (!readData.eof()) {
//			std::bitset<32> temp;
//			readData.read(reinterpret_cast<char*>(&temp), sizeof(temp));
//			binaryData.push_back(temp);
//		}
//
//		readData.close();
//
//		for (const auto& item : binaryData) {
//			std::cout << item << std::endl;
//		}
//	}
//	else {
//		std::cerr << "data.bin cannot opened!!" << std::endl;
//	}
//}
