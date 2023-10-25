//#include <iostream>
//#include <fstream>
//#include <cstdint>
//
//#include "Functions.h"
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



//#include <iostream>
//#include <fstream>
//#include "json.hpp"
//#include <vector>
//#include "Functions.h"
//#include "Deserializer.h"
//using json = nlohmann::json;
//
//int main() {
//    std::ifstream file("deneme.json"); // JSON dosyanýzýn adýný buraya ekleyin
//
//    if (!file.is_open()) {
//        std::cerr << "Dosya açýlamadý." << std::endl;
//        return 1;
//    }
//
//    json jsonData;
//    file >> jsonData;
//
//    uint32_t sourceItemId = jsonData["SourceCriteria"]["SourceItemId"];
//
//    SourceCriteria sourceCriteria;
//    sourceCriteria.SetSourceId(sourceItemId);
//
//    std::cout << "Source Item Id : " << sourceCriteria.GetSourceId() << std::endl;
//
//    //std::vector<CostInfo> costInfos;
//    //for (size_t i = 0; i < 2; i++)
//    //{
//    //    int costType = jsonData["SourceCriteria"]["CostInfos"]["CostType"][i];
//    //    int32_t costValue = jsonData["SourceCriteria"]["CostInfos"]["CostValue"][i];
//    //    std::cout << "Cost Type: " << costType << "Cost Value: " << costValue << std::endl;
//    //}
//
//    for (const auto& costInfo : jsonData["SourceCriteria"]["CostInfos"]) {
//        int costType = costInfo["CostType"];
//        int costValue = costInfo["CostValue"];
//        std::cout << "Cost Type: " << costType << " Cost Value: " << costValue << std::endl;
//    }
//
//}