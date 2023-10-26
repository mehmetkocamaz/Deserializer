#include <iostream>
#include <fstream>
#include "json.hpp"
#include <vector>
#include "Deserializer.h"
using json = nlohmann::json;

void DeserializerFunc(DeserializerManager&);

int main() {

    DeserializerManager obj;
    DeserializerFunc(obj);
    obj.DisplayScreen();
}

void DeserializerFunc(DeserializerManager& myDeserializerManagerClass) {
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

    for (const json& combineInfos : jsonData["CombineInfo"])
    {
        uint32_t targetItemId = combineInfos["TargetItemId"];
        myCombineClass.SetTargetItemId(targetItemId);
        std::cout << "Combine info in" << std::endl;
        std::cout << myCombineClass.GetTargetItemId() << std::endl;

        for (const json& combineCriterias : combineInfos["CombineCriterias"])
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
                mySourceCriteriaClass.SetSourceItemId(sourceItemId);
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
        myDeserializerManagerClass.SetCombineInfos(myCombineClass);
    }

    std::cout << myCombineClass.GetCombineCriterias().at(0).GetSourceCriterias().at(1).GetSourceItemId() << std::endl;
    std::cin.get();

}