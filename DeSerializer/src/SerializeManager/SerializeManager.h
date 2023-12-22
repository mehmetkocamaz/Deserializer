#pragma once
#include "SerializeSpec.h"
#include "BinaryManager/BinaryManager.h"
#include "Options/SaveOptions.h"


enum class Enum_SerializationStatus
{
	SUCCESS = 0,
	FAIL = 1,
	UNSUPPORTED = 2,
	OPEN_FILE_ERROR = 3,
	EMPTY_BUFFER = 4,
	COMPRESS_FAIL = 5,
	TYPE_NONE = 6,
	// MORE ERROR TYPES..
};


class SerializerManager {
public:
	SerializerManager(SerializeSpec SerializeSpecification)
		: m_SerializeSpecification(SerializeSpecification) { }
	Enum_SerializationStatus Serialize();
	Enum_SerializationStatus ProcessForSave(SaveOptions saveOptions);
	Enum_SerializationStatus CheckForNone();

private:
	Enum_SerializationStatus JsonSerialize();
	Enum_SerializationStatus BinarySerialize();
	std::vector<std::bitset<32>> m_BinaryData;
	std::vector<uint8_t> m_BinaryDataAsByteArray;
	std::vector<uint8_t> m_CompressedData;
	std::vector<uint8_t> m_DeCompressedData;
	SerializeSpec m_SerializeSpecification;
	Enum_SerializationStatus Save(std::filesystem::path filePath);
	Enum_SerializationStatus EncryptionTest();
};