#pragma once
#include "ValidatorOutput.h"
#include "E_InputType.h"

class CombineInfoValidator
{
public:
	// Delete copy constructor and copy assignment operator
	CombineInfoValidator(CombineInfoValidator const&) = delete;
	void operator=(CombineInfoValidator const&) = delete;

	// Public method to get the instance of the Singleton
	static CombineInfoValidator& Instance() {
		static CombineInfoValidator instance; // Guaranteed to be destroyed, and instantiated on first use
		return instance;
	}

private:
	CombineInfoValidator() {};

public:
	template<typename T>
	ValidatorOutput Validate(E_InputType p_InputType, T p_Input)
	{
		switch (p_InputType)
		{
		case E_InputType::TargetItemId: return ValidateTargetItemId(p_Input);
		case E_InputType::SourceItemId: return ValidateSourceItemId(p_Input);
		case E_InputType::TargetRequirementValue: return ValidateRequirementValue(p_Input);
		case E_InputType::SourceRequirementValue: return ValidateRequirementValue(p_Input);
		case E_InputType::SourceProbabilityValue: return ValidateProbabilityValue(p_Input);
		case E_InputType::SourceCostValue: return ValidateCostValue(p_Input);
		case E_InputType::TargetRequirementType: return ValidateRequirementType(p_Input);
		case E_InputType::SourceRequirementType: return ValidateRequirementType(p_Input);
		case E_InputType::SourceCostType: return ValidateCostType(p_Input);
		}
	}
private:
	template<typename T>
	ValidatorOutput ValidateTargetItemId(T p_Input)
	{
		if (p_Input > 0 && p_Input < 10000)
			return { .m_Result = true };
		else
			return { .m_Result = false, .m_Message = "Error on.." };
	}
	template<typename T>
	ValidatorOutput ValidateSourceItemId(T p_Input)
	{
		if (p_Input > 0 && p_Input < 10000)
			return { .m_Result = true };
		else
			return { .m_Result = false, .m_Message = "Error on.." };
	}
	template<typename T>
	ValidatorOutput ValidateRequirementValue(T p_Input) {
		if (p_Input > 0 && p_Input < 10000)
			return { .m_Result = true };
		else
			return { .m_Result = false, .m_Message = "Error on.." };
	}	
	template<typename T>
	ValidatorOutput ValidateProbabilityValue(T p_Input) {
		if (p_Input > 0 && p_Input < 10000)
			return { .m_Result = true };
		else
			return { .m_Result = false, .m_Message = "Error on.." };
	}
	template<typename T>
	ValidatorOutput ValidateCostValue(T p_Input) {
		if (p_Input > 0 && p_Input < 10000)
			return { .m_Result = true };
		else
			return { .m_Result = false, .m_Message = "Error on.." };
	}
	template<typename T>
	ValidatorOutput ValidateRequirementType(T p_Input) {
		if (p_Input != 4)
			return { .m_Result = true };
		else
			return { .m_Result = false, .m_Message = "Error on.." };
	}
	template<typename T>
	ValidatorOutput ValidateCostType(T p_Input) {
		if (p_Input != 4)
			return { .m_Result = true };
		else
			return { .m_Result = false, .m_Message = "Error on.." };
	}
};