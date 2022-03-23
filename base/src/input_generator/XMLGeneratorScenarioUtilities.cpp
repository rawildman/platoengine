/*
 * XMLGeneratorScenarioUtilities.cpp
 *
 *  Created on: Mar 23, 2022
 *
 */

#include <algorithm>

#include "XMLGeneratorDataStruct.hpp"
#include "XMLGeneratorParserUtilities.hpp"
#include "XMLGeneratorScenarioUtilities.hpp"

namespace XMLGen
{

/******************************************************************************/
std::vector<std::string> 
get_unique_scenario_ids
(const std::string& aTargetCodeName,
 const XMLGen::InputData& aInputMetaData)
{
    std::vector<std::string> tUniqueScenarioIDs;
    auto tLowerTargetCodeName = XMLGen::to_lower(aTargetCodeName);

    for(auto& tID : aInputMetaData.objective.criteriaIDs)
    {
        auto tIndex = &tID - &aInputMetaData.objective.criteriaIDs[0];
        auto tServiceID = aInputMetaData.objective.serviceIDs[tIndex];
        auto tLowerCode = XMLGen::to_lower(aInputMetaData.service(tServiceID).code());
        if(tLowerCode == tLowerTargetCodeName)
        {
            auto tScenarioID = aInputMetaData.objective.scenarioIDs[tIndex];
            if(std::find(tUniqueScenarioIDs.begin(), tUniqueScenarioIDs.end(), tScenarioID) == tUniqueScenarioIDs.end())
            {
                tUniqueScenarioIDs.push_back(tScenarioID);   
            }
        }
    }

    return tUniqueScenarioIDs;
}
// function get_unique_scenario_ids
/******************************************************************************/

}
 // namespace XMLGen