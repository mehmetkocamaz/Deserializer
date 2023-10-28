#pragma once
#include <filesystem>

enum class Enum_ContType
{
	JSON = 1,
	BINARY = 2
};

struct SerializeSpec
{
	Enum_ContType m_FileType;
	std::filesystem::path m_FilePath;
};