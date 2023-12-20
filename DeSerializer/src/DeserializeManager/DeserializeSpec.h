#pragma once
#include <filesystem>
#include "Options/SaveOptions.h"

enum class Enum_DeserializeContentType
{
	JSON = 1,
	BINARY = 2
};

struct DeserializeSpec
{
	Enum_DeserializeContentType m_FileType;
	SaveOptions m_SaveOptions;
};