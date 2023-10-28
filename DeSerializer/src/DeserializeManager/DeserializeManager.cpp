#include "DeserializeManager.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <bitset>
using json = nlohmann::json;

// DESERIALIZER MANAGER FUNCTION DECLARATIONS

Enum_DeserializationStatus DeserializerManager::Deserialize(const DeserializeSpec& deserializeSpec)
{
	switch (deserializeSpec.m_FileType)
	{
	case Enum_ContentType::JSON:
		return JsonDeserialize(deserializeSpec.m_FilePath);
	case Enum_ContentType::BINARY:
		return BinaryDeserialize(deserializeSpec.m_FilePath);
	default:
		return Enum_DeserializationStatus::UNSUPPORTED;
	}
}

Enum_DeserializationStatus DeserializerManager::JsonDeserialize( std::filesystem::path filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "The json file cannot opened." << std::endl;
		return Enum_DeserializationStatus::OPEN_FILE_ERROR;
	}

	try
	{
		json jsonData;
		file >> jsonData;
		SourceCriteria mySourceCriteriaClass;

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


Enum_DeserializationStatus DeserializerManager::BinaryDeserialize(std::filesystem::path filePath)
{
	// Binary implemenentation
	std::ifstream readData(filePath, std::ios::binary);
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
		// Display Screen
		for (const auto& item : binaryData) {
			std::cout << item << std::endl;
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
