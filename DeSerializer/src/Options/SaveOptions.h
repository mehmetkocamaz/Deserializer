#pragma once
#define BIT(X) 1<<X
#include <cstdint>
#include <filesystem>

enum Enum_Save : uint32_t
{
	E_Default = BIT(0),
	E_Compress = BIT(1),
	E_Decompress = BIT(2),
	E_XorFilter = BIT(3)
};

struct SaveOptions
{
	Enum_Save m_SaveFlags = E_Default;
	uint8_t m_XorKey = 0;
	std::filesystem::path m_FilePath;

};