#pragma once
#include "DataClasses/CombineInfo.h"
class CombineInfoRoot
{
public:
	// Delete copy constructor and copy assignment operator
	CombineInfoRoot(CombineInfoRoot const&) = delete;
	void operator=(CombineInfoRoot const&) = delete;

	// Public method to get the instance of the Singleton
	static CombineInfoRoot& Instance() {
		static CombineInfoRoot instance; // Guaranteed to be destroyed, and instantiated on first use
		return instance;
	}

private:
	CombineInfoRoot() {};
public:
	std::vector<CombineInfo> m_CombineInfos;
};