#pragma once
#include <filesystem>
#include "DataClasses/CombineInfo.h"
enum class Enum_SerizalizeContentType
{
	JSON = 1,
	BINARY = 2
};

struct SerializeSpec
{
	Enum_SerizalizeContentType m_ContentType;
	const std::vector<CombineInfo>* m_CombineInfos;
};