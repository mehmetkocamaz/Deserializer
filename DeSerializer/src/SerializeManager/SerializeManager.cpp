#pragma once
#include <iostream>
#include <fstream>
#include "SerializeManager.h"
#include "BinaryManager/BinaryManager.h"
#include "DeserializeManager/DeserializeManager.h"

Enum_SerializationStatus SerializerManager::Serialize(const SerializeSpec& serializeSpec) {
	switch (serializeSpec.m_FileType)
	{
	case Enum_ContType::JSON:
		return JsonSerialize(serializeSpec.m_FilePath);
	case Enum_ContType::BINARY:
		return BinarySerialize(serializeSpec.m_FilePath);
	default:
		return Enum_SerializationStatus::UNSUPPORTED;
	}
}

Enum_SerializationStatus SerializerManager::JsonSerialize(std::filesystem::path filePath) {
	return Enum_SerializationStatus::SUCCESS;
}

Enum_SerializationStatus SerializerManager::BinarySerialize(std::filesystem::path filePath) {
	std::ofstream binaryFile(filePath, std::ios::binary);

	if (!binaryFile.is_open())
	{
		std::cerr << "data.bin cannot opened!!" << std::endl;
		return Enum_SerializationStatus::OPEN_FILE_ERROR;
	}
	try
	{
		BinaryManager binaryData; // BINARY DATA HAVE NO MEMBERS TO WRITE \\ DIDNT IMPLEMENTED CAUSE OF HIERARCHY 
		for (const std::bitset<32> data : binaryData.GetBinaryData()) {
			binaryFile.write(reinterpret_cast<const char*>(&data), sizeof(data));
		}
		binaryFile.close();
	}
	catch (const std::exception&)
	{
		return Enum_SerializationStatus::FAIL;
	}

	return Enum_SerializationStatus::SUCCESS;
}