#pragma once
#include "SerializeSpec.h"
#include "BinaryManager/BinaryManager.h"

enum class Enum_SerializationStatus
{
	SUCCESS = 0,
	FAIL = 1,
	UNSUPPORTED = 2,
	OPEN_FILE_ERROR = 3
	// MORE ERROR TYPES..
};

class SerializerManager {
public:
	SerializerManager(SerializeSpec SerializeSpecification)
		: m_SerializeSpecification(SerializeSpecification) { }
	Enum_SerializationStatus Serialize();

private:
	Enum_SerializationStatus JsonSerialize();
	Enum_SerializationStatus BinarySerialize();
	std::vector<std::bitset<32>> m_BinaryData;
	SerializeSpec m_SerializeSpecification;
};