#pragma once
#include <cstdint>
enum class E_InputType : uint32_t
{
	TargetItemId,
	SourceItemId,
	TargetRequirementValue,
	SourceRequirementValue,
	SourceCostValue,
	SourceProbabilityValue,
	TargetRequirementType,
	SourceRequirementType,
	SourceCostType,
};