//#include <iostream>
//#include <fstream>
//#include <cstdint>
//
//#include "Deserializer.h"
//#include"json.hpp"
//using json = nlohmann::json;
//
//int main() {
//	std::ifstream file("example.json");
//	if (!file.is_open()) {
//		std::cerr << "JSON FILE CANNOT OPENED" << std::endl;
//		return 1;
//	}
//
//	json jsonData;
//	file >> jsonData;
//
//	// Cost Info Struct
//	int costType = jsonData["CostInfo"]["CostType"];
//	int32_t costValue = jsonData["CostInfo"]["CostValue"];
//
//	// Requiremenet Info Struct
//	int requirementType = jsonData["RequirementInfo"]["RequirementType"];
//	int32_t requirementValue = jsonData["RequirementInfo"]["RequirementValue"];
//
//	
//	CostInfo costInfo1;
//	RequirementInfo requirementInfo1;
//
//	CostInfoFiller(costInfo1, costType, costValue);
//	RequirementInfoFiller(requirementInfo1, requirementType, requirementValue);
//
//	std::cout << "CostInfo:\nCostType: " << static_cast<int>(costInfo1.m_CostType) << "\nCostValue: " << costInfo1.m_CostValue << std::endl;
//	std::cout << "RequirementInfo:\nRequirementType: " << static_cast<int>(requirementInfo1.m_RequirementType) << "\nRequirementValue: " << requirementInfo1.m_RequirementValue << std::endl;
//
//	std::ifstream file2("target.json");
//	if (!file2.is_open()) {
//		std::cerr << "JSON FILE CANNOT OPENED" << std::endl;
//		return 1;
//	}
//
//	json jsonData2;
//	file2 >> jsonData2;
//
//	uint32_t targetItemID = jsonData2["CombineInfo"]["TargetItemId"];
//	
//	std::vector<CombineCriteria> combineCriterias;
//	std::vector<RequirementInfo> targetRequirementInfos;
//
//	CombineInfo obj1;
//	obj1.SetTargetId(targetItemID);
//
//	std::cout << obj1.GetTargetId() << std::endl;
//}



#include <iostream>
#include <fstream>
#include "json.hpp"
#include <vector>
#include "Deserializer.h"
using json = nlohmann::json;

void DeserializerFunc();

int main() {
    //std::ifstream file("example.json");

    //if (!file.is_open()) {
    //    std::cerr << "Dosya açýlamadý." << std::endl;
    //    return 1;
    //}

    //json jsonData;
    //file >> jsonData;

    //uint32_t sourceItemId = jsonData["SourceCriteria"]["SourceItemId"];

    //SourceCriteria sourceCriteria;
    //sourceCriteria.SetSourceItemId(sourceItemId);

    //std::cout << "Source Item Id : " << sourceCriteria.GetSourceItemId() << std::endl;


    //for (const auto& costInfo : jsonData["SourceCriteria"]["CostInfos"]) {
    //    int costType = costInfo["CostType"];
    //    int costValue = costInfo["CostValue"];
    //    std::cout << "Cost Type: " << costType << " Cost Value: " << costValue << std::endl;
    //}  
    //
    //for (const json& requirementInfo : jsonData["SourceCriteria"]["RequirementInfos"]) {
    //    int costType = requirementInfo["RequirementType"];
    //    int costValue = requirementInfo["RequirementValue"];
    //    std::cout << "Requirement Type: " << costType << " Requirement Value: " << costValue << std::endl;
    //}
    DeserializerFunc();

}

void DeserializerFunc() {
    std::ifstream file("target.json"); 
    if (!file.is_open()) {
        std::cerr << "The json file cannot opened." << std::endl;
        return;
    }

    json jsonData;
    file >> jsonData;

    CombineInfo myCombineClass;
    CombineCriteria myCombineCriteriaClass;
    SourceCriteria mySourceCriteriaClass;

    if (myCombineClass.SetTargetItemId(jsonData["CombineInfo"]["TargetItemId"])) {
        std::cout << "Combine info in" << std::endl;
        std::cout << myCombineClass.GetTargetItemId() << std::endl;

        for (const json& combineCriterias : jsonData["CombineInfo"]["CombineCriterias"])
        {
            for (const json& targetRequirementInfos : combineCriterias["TargetRequirementInfos"]) {
                int requirementType = targetRequirementInfos["RequirementType"];
                int32_t requirementValue = targetRequirementInfos["RequirementValue"];
                RequirementInfo obj;
                obj.m_RequirementType = static_cast<Enum_Requirement>(requirementType);
                obj.m_RequirementValue = requirementValue;
                myCombineCriteriaClass.SetTargetRequirementInfo(obj);
            }
            for (const json& sourceCriterias : combineCriterias["SourceCriterias"]) {
                uint32_t sourceItemId = sourceCriterias["SourceItemId"];
                for (const json& costInfos : sourceCriterias["CostInfos"])
                {
                    int costType = costInfos["CostType"];
                    int32_t costValue = costInfos["CostValue"];
                    CostInfo obj;
                    obj.m_CostType = static_cast<Enum_Cost>(costType);
                    obj.m_CostValue = costValue;
                    mySourceCriteriaClass.SetCostInfo(obj);
                }
                for (const json& sourceRequirementInfos : sourceCriterias["SourceRequirementInfos"])
                {
                    int requirementType = sourceRequirementInfos["RequirementType"];
                    int32_t requirementValue = sourceRequirementInfos["RequirementValue"];
                    RequirementInfo obj;
                    obj.m_RequirementType = static_cast<Enum_Requirement>(requirementType);
                    obj.m_RequirementValue = requirementValue;
                    mySourceCriteriaClass.SetSourceRequirementInfo(obj);
                }
                myCombineCriteriaClass.SetSourceCriterias(mySourceCriteriaClass);
            }
            myCombineClass.SetCombineCriterias(myCombineCriteriaClass);
        }
    }
    else
    {
        std::cout << "Combine info out" << std::endl;
        return;
    }

}