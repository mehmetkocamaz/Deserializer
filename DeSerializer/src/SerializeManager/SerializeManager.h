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

	Enum_SerializationStatus Serialize(const SerializeSpec& serializeSpec);

private:
	Enum_SerializationStatus JsonSerialize(const SerializeSpec& serializeSpec);
	Enum_SerializationStatus BinarySerialize(const SerializeSpec& serializeSpec);
	std::vector<std::bitset<32>> m_BinaryData;

};