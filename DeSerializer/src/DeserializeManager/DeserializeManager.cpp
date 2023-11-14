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
					combineCriteriaClass.SetTargetRequirementInfo(reqObj);
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
					combineCriteriaClass.SetSourceCriterias(sourceCriteriaClass);
				}
				combineInfoClass.SetCombineCriterias(combineCriteriaClass);
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
	if (buffer.size() > 4)
	{
		Utils::ReadFromBuffer(buffer, decompressedBufferSize, offset);
		buffer.erase(buffer.begin(), buffer.begin() + 4);
	}

	{
		if (m_DeserializeSpecification.m_SaveOptions.m_SaveFlags & Enum_Save::E_XorFilter)
			Utils::ApplyXorFilter(buffer, m_DeserializeSpecification.m_SaveOptions.m_XorKey);

		if (m_DeserializeSpecification.m_SaveOptions.m_SaveFlags & Enum_Save::E_Decompress)
		{
			decompressedDataBuffer.resize(decompressedBufferSize);
			Utils::Decompress(buffer, decompressedDataBuffer);
		}
		else
		{
			decompressedDataBuffer = buffer;
		}
	}

	{
		offset = 0;
		uint32_t combineInfoSize = 0;
		Utils::ReadFromBuffer(decompressedDataBuffer,combineInfoSize,offset);
		for (;offset < decompressedDataBuffer.size();)
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
					combineCriteria.SetTargetRequirementInfo(requirementInfo);
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
					combineCriteria.SetSourceCriterias(sourceCriteria);
				}
				combineInfo.SetCombineCriterias(combineCriteria);
			}
			m_CombineInfos.push_back(combineInfo);
		}
	}

	return Enum_DeserializationStatus::SUCCESS;
}

// ~Setters
void DeserializerManager::SetCombineInfos(CombineInfo& obj) {
	m_CombineInfos.push_back(obj);
}
// ~Getters
std::vector<CombineInfo> DeserializerManager::GetCombineInfos() const {
	return m_CombineInfos;
}

void DeserializerManager::DisplayScreen() const {
	for (size_t i = 0; i < m_CombineInfos.size(); i++) // LOOP FOR COMBINE INFO VECTOR
	{
		std::cout << "------------------------------------------" << std::endl;
		std::cout << "Combine Info: \n" << "Target Item ID: " << m_CombineInfos.at(i).GetTargetItemId() << std::endl;
		std::cout << "------------------------------------------" << std::endl;
		std::cout << "Combine Criterias: " << std::endl;
		for (size_t j = 0; j < m_CombineInfos.at(i).GetCombineCriterias().size(); j++) // LOOP FOR COMBINE CRITERIA VECTOR
		{
			std::cout << "------------------------------------------" << std::endl;
			std::cout << "Target Requirement Infos: " << std::endl;
			for (size_t k = 0; k < m_CombineInfos.at(i).GetCombineCriterias().at(j).GetTargetRequirementInfo().size(); k++) // LOOP FOR TARGET REQUIREMENT INFOS
			{
				std::cout << "Target Requirement Type: " << static_cast<int>(m_CombineInfos.at(i).GetCombineCriterias().at(j).GetTargetRequirementInfo().at(k).m_RequirementType) << std::endl;
				std::cout << "Target Requirement Value: " << m_CombineInfos.at(i).GetCombineCriterias().at(j).GetTargetRequirementInfo().at(k).m_RequirementValue << std::endl;
			}
			std::cout << "------------------------------------------" << std::endl;
			std::cout << "Source Criterias: " << std::endl;
			for (size_t k = 0; k < m_CombineInfos.at(i).GetCombineCriterias().at(j).GetSourceCriterias().size(); k++) // LOOP FOR SOURCE CRITERIAS
			{
				std::cout << "------------------------------------------" << std::endl;
				std::cout << "Source Item ID: " << m_CombineInfos.at(i).GetCombineCriterias().at(j).GetSourceCriterias().at(k).GetSourceItemId() << std::endl;
				std::cout << "------------------------------------------" << std::endl;
				std::cout << "Source Cost Infos: " << std::endl;
				for (size_t l = 0; l < m_CombineInfos.at(i).GetCombineCriterias().at(j).GetSourceCriterias().at(k).GetCostInfos().size(); l++) // LOOP FOR SOURCE COST INFOS
				{
					std::cout << "Cost Type: " << static_cast<int>(m_CombineInfos.at(i).GetCombineCriterias().at(j).GetSourceCriterias().at(k).GetCostInfos().at(l).m_CostType) << std::endl;
					std::cout << "Cost Value: " << m_CombineInfos.at(i).GetCombineCriterias().at(j).GetSourceCriterias().at(k).GetCostInfos().at(l).m_CostValue << std::endl;
				}
				std::cout << "------------------------------------------" << std::endl;
				std::cout << "Source Requirement Infos: " << std::endl;
				for (size_t l = 0; l < m_CombineInfos.at(i).GetCombineCriterias().at(j).GetSourceCriterias().at(k).GetSourceRequirementInfos().size(); l++) // LOOP FOR SOURCE REQUIREMENT INFOS
				{
					std::cout << "Requirement Type: " << static_cast<int>(m_CombineInfos.at(i).GetCombineCriterias().at(j).GetSourceCriterias().at(k).GetSourceRequirementInfos().at(l).m_RequirementType) << std::endl;
					std::cout << "Requirement Value: " << m_CombineInfos.at(i).GetCombineCriterias().at(j).GetSourceCriterias().at(k).GetSourceRequirementInfos().at(l).m_RequirementValue << std::endl;
				}
			}
		}
	}
}


//void DeserializerManager::ReadUint32_t(const std::vector<std::bitset<32>>& buffer, uint32_t& destination, uint32_t& offset, uint32_t& previousOffset) {
//	
//	destination = static_cast<uint32_t>(buffer[offset++].to_ulong());
//	previousOffset++;
//}
//void DeserializerManager::ReadInt32_t(const std::vector<std::bitset<32>>& buffer, int32_t& destination, uint32_t& offset, uint32_t& previousOffset) {
//	destination = static_cast<int32_t>(buffer[offset++].to_ulong());
//	previousOffset++;
//}
//void DeserializerManager::ReadEnum_Cost(const std::vector<std::bitset<32>>& buffer, Enum_Cost& destination, uint32_t& offset, uint32_t& previousOffset) {
//	int32_t temp = static_cast<int32_t>(buffer[offset++].to_ulong());
//	switch (temp)
//	{
//	case 0:
//		destination = Enum_Cost::Silver;
//		break;
//	case 1:
//		destination = Enum_Cost::Billion;
//		break;
//	case 2:
//		destination = Enum_Cost::ContributionPoint;
//		break;
//	case 3:
//		destination = Enum_Cost::BloodPoint;
//		break;
//	default:
//		std::cerr << "There is an error about deserializing binary data.";
//		break;
//	}
//	previousOffset++;
//}
//
//void DeserializerManager::ReadEnum_Requirement(const std::vector<std::bitset<32>>& buffer, Enum_Requirement& destination, uint32_t& offset, uint32_t& previousOffset) {
//	int32_t temp = static_cast<int32_t>(buffer[offset++].to_ulong());
//	switch (temp)
//	{
//	case 0:
//		destination = Enum_Requirement::Enchanment;
//		break;
//	case 1:
//		destination = Enum_Requirement::Combine;
//		break;
//	case 2:
//		destination = Enum_Requirement::Refine;
//		break;
//	case 3:
//		destination = Enum_Requirement::Socket;
//		break;
//	default:
//		std::cerr << "There is an error about deserializing binary data.";
//		break;
//	}
//	previousOffset++;
//}
//
//void DeserializerManager::ReadEnum_Probability(const std::vector<std::bitset<32>>& buffer, Enum_Probability& destination, uint32_t& offset, uint32_t& previousOffset) {
//	int32_t temp = static_cast<int32_t>(buffer[offset++].to_ulong());
//	switch (temp)
//	{
//	case 0:
//		destination = Enum_Probability::Success;
//		break;
//	case 1:
//		destination = Enum_Probability::Fail;
//		break;
//	case 2:
//		destination = Enum_Probability::Break;
//		break;
//	default:
//		std::cerr << "There is an error about deserializing binary data.";
//		break;
//	}
//	previousOffset++;
//}

