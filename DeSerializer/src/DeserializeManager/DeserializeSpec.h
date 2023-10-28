#pragma once
#include <filesystem>

enum class Enum_DeserializeContentType
{
	JSON = 1,
	BINARY = 2
};

struct DeserializeSpec
{
	Enum_DeserializeContentType m_FileType;
	std::filesystem::path m_FilePath;
};