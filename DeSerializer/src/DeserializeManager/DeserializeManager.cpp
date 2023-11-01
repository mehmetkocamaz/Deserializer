#include "DeserializeManager.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <bitset>

using json = nlohmann::json;
void ReadUint32_t(const std::vector<std::bitset<32>>& buffer, uint32_t& destination, uint32_t& offset, uint32_t& previousOffset);
void ReadInt32_t(const std::vector<std::bitset<32>>& buffer, int32_t& destination, uint32_t& offset, uint32_t previousOffset);
void ReadEnum_Cost(const std::vector<std::bitset<32>>& buffer, Enum_Cost& destination, uint32_t& offset, uint32_t previousOffset);
void ReadEnum_Requirement(const std::vector<std::bitset<32>>& buffer, Enum_Requirement& destination, uint32_t& offset, uint32_t previousOffset);


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
	std::ifstream file(m_DeserializeSpecification.m_FilePath);
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
	
	std::ifstream readData(m_DeserializeSpecification.m_FilePath, std::ios::binary);
	if (!readData.is_open())
	{
		std::cerr << "data.bin cannot opened!!" << std::endl;
		return Enum_DeserializationStatus::OPEN_FILE_ERROR;
	}
	try
	{
		std::vector<std::bitset<32>> binaryData;
		while (!readData.eof()) {
			std::bitset<32> temp;
			readData.read(reinterpret_cast<char*>(&temp), sizeof(temp));
			binaryData.push_back(temp);
		}
		readData.close();

		std::vector<uint32_t> totalBytesRead;
		uint32_t combineInfoSize = binaryData[0].to_ulong();

		for (int32_t i = 0; i < combineInfoSize; i++) {
			CombineInfo combineInfo;
			uint32_t previousOffset = i == 0 ? 0 : totalBytesRead[i - 1];
			
			totalBytesRead.push_back(0);
			uint32_t offset = 1 + previousOffset;
			ReadUint32_t(binaryData, combineInfo.GetTargetItemIdRef(), offset, previousOffset);

			uint32_t combineCriteriaSize;
			ReadUint32_t(binaryData, combineCriteriaSize, offset, previousOffset);
			for (int32_t j = 0; j < combineCriteriaSize; j++)
			{
				CombineCriteria combineCriteria;
				uint32_t targetRequirementInfoSize;
				
				ReadUint32_t(binaryData, targetRequirementInfoSize, offset, previousOffset);
				for (int32_t k = 0; k < targetRequirementInfoSize; k++)
				{
					RequirementInfo requirementInfo;
					ReadEnum_Requirement(binaryData, requirementInfo.GetRequirementTypeRef(), offset, previousOffset);
					ReadUint32_t(binaryData, requirementInfo.GetRequirementValueRef(), offset, previousOffset);
					combineCriteria.SetTargetRequirementInfo(requirementInfo);
				}
				uint32_t sourceCriteriaSize;
				ReadUint32_t(binaryData, sourceCriteriaSize, offset, previousOffset);
				for (int32_t k = 0; k < sourceCriteriaSize; k++) {
					SourceCriteria sourceCriteria;
					ReadUint32_t(binaryData, sourceCriteria.GetSourceItemIdRef(), offset, previousOffset);
					int32_t costInfoSize;
					ReadInt32_t(binaryData, costInfoSize, offset, previousOffset);
					for (int32_t m = 0; m < costInfoSize; m++) {
						CostInfo costInfo;
						ReadEnum_Cost(binaryData, costInfo.GetCostTypeRef(), offset, previousOffset);
						ReadUint32_t(binaryData, costInfo.GetCostValueRef(), offset, previousOffset);
						sourceCriteria.SetCostInfo(costInfo);
					}
					int32_t requirementInfoSize;
					ReadInt32_t(binaryData, requirementInfoSize, offset, previousOffset);
					for (int32_t m = 0; m < requirementInfoSize; m++) {
						RequirementInfo requirementInfo;
						ReadEnum_Requirement(binaryData, requirementInfo.GetRequirementTypeRef(), offset, previousOffset);
						ReadUint32_t(binaryData, requirementInfo.GetRequirementValueRef(), offset, previousOffset);
						sourceCriteria.SetSourceRequirementInfo(requirementInfo);
					}
					combineCriteria.SetSourceCriterias(sourceCriteria);
				}
				combineInfo.SetCombineCriterias(combineCriteria);
			}
			m_CombineInfos.push_back(combineInfo);
		}

	}
	catch (const std::exception&)
	{
		return Enum_DeserializationStatus::FAIL;
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

void ReadUint32_t(const std::vector<std::bitset<32>>& buffer, uint32_t& destination, uint32_t& offset, uint32_t& previousOffset) {
	
	destination = static_cast<uint32_t>(buffer[offset++].to_ulong());
	previousOffset++;
}
void ReadInt32_t(const std::vector<std::bitset<32>>& buffer, int32_t& destination, uint32_t& offset, uint32_t previousOffset) {
	destination = static_cast<int32_t>(buffer[offset++].to_ulong());
	previousOffset++;
}
void ReadEnum_Cost(const std::vector<std::bitset<32>>& buffer, Enum_Cost& destination, uint32_t& offset, uint32_t previousOffset) {
	int32_t temp = static_cast<int32_t>(buffer[offset++].to_ulong());
	switch (temp)
	{
	case 0:
		destination = Enum_Cost::Silver;
		break;
	case 1:
		destination = Enum_Cost::Billion;
		break;
	case 2:
		destination = Enum_Cost::ContributionPoint;
		break;
	case 3:
		destination = Enum_Cost::BloodPoint;
		break;
	default:
		std::cerr << "There is an error about deserializing binary data.";
		break;
	}
	previousOffset++;
}

void ReadEnum_Requirement(const std::vector<std::bitset<32>>& buffer, Enum_Requirement& destination, uint32_t& offset, uint32_t previousOffset) {
	int32_t temp = static_cast<int32_t>(buffer[offset++].to_ulong());
	switch (temp)
	{
	case 0:
		destination = Enum_Requirement::Enchanment;
		break;
	case 1:
		destination = Enum_Requirement::Combine;
		break;
	case 2:
		destination = Enum_Requirement::Refine;
		break;
	case 3:
		destination = Enum_Requirement::Socket;
		break;
	default:
		std::cerr << "There is an error about deserializing binary data.";
		break;
	}
	previousOffset++;
}
