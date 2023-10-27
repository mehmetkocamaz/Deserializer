#pragma once
#include <filesystem>

enum class Enum_ContentType
{
	JSON = 1,
	BINARY = 2
};

struct DeserializeSpec
{
	Enum_ContentType m_FileType;
	std::filesystem::path m_FilePath;
};