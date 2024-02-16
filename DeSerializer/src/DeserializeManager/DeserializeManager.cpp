#include "DeserializeManager.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <bitset>
#include "Utils/Utils.h"

using json = nlohmann::json;



// DESERIALIZER MANAGER FUNCTION DECLARATIONS

Enum_DeserializationStatus DeserializerManager::Deserialize()
{
	switch (m_DeserializeSpecification.m_FileType)
	{
	case Enum_DeserializeContentType::JSON:
		return JsonDeserialize();
	case Enum_DeserializeContentType::BINARY:
		return BinaryDeserialize();
	default:
		return Enum_DeserializationStatus::UNSUPPORTED;
	}
}

Enum_DeserializationStatus DeserializerManager::JsonDeserialize() {
	std::ifstream file(m_DeserializeSpecification.m_SaveOptions.m_FilePath);
	if (!file.is_open()) {
		std::cerr << "The json file cannot opened." << std::endl;
		return Enum_DeserializationStatus::OPEN_FILE_ERROR;
	}

	try
	{
		json jsonData;
		file >> jsonData;

		for (const json& combineInfo : jsonData["CombineInfos"])
		{
			CombineInfo combineInfoClass;
			uint32_t targetItemId = combineInfo["TargetItemId"];
			combineInfoClass.SetTargetItemId(targetItemId);

			for (const json& combineCriterias : combineInfo["CombineCriterias"])
			{
				CombineCriteria combineCriteriaClass;
				for (const json& targetRequirementInfos : combineCriterias["TargetRequirementInfos"]) {
					int requirementType = targetRequirementInfos["RequirementType"];
					RequirementInfo reqObj;
					reqObj.m_RequirementType = static_cast<Enum_Requirement>(requirementType);
					reqObj.m_RequirementValue = targetRequirementInfos["RequirementValue"];
					combineCriteriaClass.PushTargetRequirementInfo(reqObj);
				}
				for (const json& sourceCriterias : combineCriterias["SourceCriterias"]) {
					SourceCriteria sourceCriteriaClass;
					uint32_t sourceItemId = sourceCriterias["SourceItemId"];
					sourceCriteriaClass.SetSourceItemId(sourceItemId);
					for (const json& costInfos : sourceCriterias["CostInfos"])
					{
						int costType = costInfos["CostType"];
						int32_t costValue = costInfos["CostValue"];

						CostInfo costObj;
						costObj.m_CostType = static_cast<Enum_Cost>(costType);
						costObj.m_CostValue = costValue;
						sourceCriteriaClass.SetCostInfo(costObj);
					}
					for (const json& probabilityInfos : sourceCriterias["ProbabilityInfos"]) {

						int probabilityType = probabilityInfos["ProbabilityType"];
						uint32_t probabilityValue = probabilityInfos["ProbabilityValue"];
						ProbabilityInfo probabilityObj;

						probabilityObj.m_ProbabilityType = static_cast<Enum_Probability>(probabilityType);
						probabilityObj.m_ProbabilityValue = probabilityValue;
						sourceCriteriaClass.SetProbabilityInfo(probabilityObj);
					}
					for (const json& sourceRequirementInfos : sourceCriterias["SourceRequirementInfos"])
					{
						int requirementType = sourceRequirementInfos["RequirementType"];
						int32_t requirementValue = sourceRequirementInfos["RequirementValue"];

						RequirementInfo reqObj;
						reqObj.m_RequirementType = static_cast<Enum_Requirement>(requirementType);
						reqObj.m_RequirementValue = requirementValue;
						sourceCriteriaClass.SetSourceRequirementInfo(reqObj);
					}
					combineCriteriaClass.PushSourceCriterias(sourceCriteriaClass);
				}
				combineInfoClass.PushCombineCriterias(combineCriteriaClass);
			}
			m_CombineInfos.push_back(combineInfoClass);
		}
	}
	catch (const std::exception&)
	{
		return Enum_DeserializationStatus::FAIL;
	}

	return Enum_DeserializationStatus::SUCCESS;
}


Enum_DeserializationStatus DeserializerManager::BinaryDeserialize()
{
	std::vector<uint8_t> buffer = Utils::ReadBytesToVector(m_DeserializeSpecification.m_SaveOptions.m_FilePath);
	std::vector<uint8_t> decompressedDataBuffer;
	uint32_t decompressedBufferSize = 0;
	size_t offset = 0;

	if (buffer.size() > 8)
	{
		int32_t magicKey = 0;
		Utils::ReadFromBuffer(buffer, decompressedBufferSize, offset);
		Utils::ReadFromBuffer(buffer, m_DeserializeSpecification.m_SaveOptions.m_MagicKey, offset);
		buffer.erase(buffer.begin(), buffer.begin() + 8);
	}

	{
		if (m_DeserializeSpecification.m_SaveOptions.m_SaveFlags & Enum_Save::E_XorFilter)
			Utils::ApplyXorFilter(buffer, m_DeserializeSpecification.m_SaveOptions.m_XorKey);
		else 
		{
			if (m_DeserializeSpecification.m_SaveOptions.m_SaveFlags & Enum_Save::E_Decompress)
				//m_DeserializeSpecification.m_SaveOptions.m_MagicKey -= 0x10000000;
			Utils::ApplyXorFilter(buffer, m_DeserializeSpecification.m_SaveOptions.m_MagicKey);
		}

		if (m_DeserializeSpecification.m_SaveOptions.m_SaveFlags & Enum_Save::E_Decompress)
		{
			decompressedDataBuffer.resize(decompressedBufferSize);
			if (!Utils::Decompress(buffer, decompressedDataBuffer)) {
				std::cout << "Data cannot decompressed." << std::endl;
			}
		}
		else
		{
			decompressedDataBuffer = buffer;
		}
	}

	{
		offset = 0;
		uint32_t combineInfoSize = 0;

		try
		{
			Utils::ReadFromBuffer(decompressedDataBuffer, combineInfoSize, offset);
			for (; offset < decompressedDataBuffer.size();)
			{
				CombineInfo combineInfo;
				Utils::ReadFromBuffer(decompressedDataBuffer, combineInfo.GetTargetItemIdRef(), offset);
				uint32_t combineCriteriaSize;
				Utils::ReadFromBuffer(decompressedDataBuffer, combineCriteriaSize, offset);
				for (int32_t j = 0; j < combineCriteriaSize; j++)
				{
					CombineCriteria combineCriteria;
					uint32_t targetRequirementInfoSize;

					Utils::ReadFromBuffer(decompressedDataBuffer, targetRequirementInfoSize, offset);
					for (int32_t k = 0; k < targetRequirementInfoSize; k++)
					{
						RequirementInfo requirementInfo;
						Utils::ReadFromBuffer(decompressedDataBuffer, requirementInfo.GetRequirementTypeRef(), offset);
						Utils::ReadFromBuffer(decompressedDataBuffer, requirementInfo.GetRequirementValueRef(), offset);
						combineCriteria.PushTargetRequirementInfo(requirementInfo);
					}
					uint32_t sourceCriteriaSize;
					Utils::ReadFromBuffer(decompressedDataBuffer, sourceCriteriaSize, offset);
					for (int32_t k = 0; k < sourceCriteriaSize; k++) {
						SourceCriteria sourceCriteria;
						Utils::ReadFromBuffer(decompressedDataBuffer, sourceCriteria.GetSourceItemIdRef(), offset);

						uint32_t costInfoSize;
						Utils::ReadFromBuffer(decompressedDataBuffer, costInfoSize, offset);
						for (int32_t m = 0; m < costInfoSize; m++) {
							CostInfo costInfo;
							Utils::ReadFromBuffer(decompressedDataBuffer, costInfo.GetCostTypeRef(), offset);
							Utils::ReadFromBuffer(decompressedDataBuffer, costInfo.GetCostValueRef(), offset);
							sourceCriteria.SetCostInfo(costInfo);
						}

						uint32_t probabilityInfoSize;
						Utils::ReadFromBuffer(decompressedDataBuffer, probabilityInfoSize, offset);
						for (int32_t m = 0; m < probabilityInfoSize; m++) {
							ProbabilityInfo probabilityInfo;
							Utils::ReadFromBuffer(decompressedDataBuffer, probabilityInfo.GetProbabiltyTypeRef(), offset);
							Utils::ReadFromBuffer(decompressedDataBuffer, probabilityInfo.GetProbabilityValueRef(), offset);
							sourceCriteria.SetProbabilityInfo(probabilityInfo);
						}

						uint32_t requirementInfoSize;
						Utils::ReadFromBuffer(decompressedDataBuffer, requirementInfoSize, offset);
						for (int32_t m = 0; m < requirementInfoSize; m++) {
							RequirementInfo requirementInfo;
							Utils::ReadFromBuffer(decompressedDataBuffer, requirementInfo.GetRequirementTypeRef(), offset);
							Utils::ReadFromBuffer(decompressedDataBuffer, requirementInfo.GetRequirementValueRef(), offset);
							sourceCriteria.SetSourceRequirementInfo(requirementInfo);
						}
						combineCriteria.PushSourceCriterias(sourceCriteria);
					}
					combineInfo.PushCombineCriterias(combineCriteria);
				}
				m_CombineInfos.push_back(combineInfo);
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "The error is " << e.what() << std::endl;
		}
	}

	return Enum_DeserializationStatus::SUCCESS;
}


std::vector<CombineInfo> DeserializerManager::GetCombineInfos() const {
	return m_CombineInfos;
}
