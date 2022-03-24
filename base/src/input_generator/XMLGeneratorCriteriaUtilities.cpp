/*
 * XMLGeneratorCriteriaUtilities.cpp
 *
 *  Created on: March 24, 2022
 */

#include "XMLGeneratorUtilities.hpp"
#include "XMLGeneratorDataStruct.hpp"
#include "XMLGeneratorCriteriaUtilities.hpp"

namespace XMLGen
{

std::vector<std::string> 
get_criteria_ids
(const std::string& aType,
 const XMLGen::InputData& aInputMetaData)
{
    std::vector<std::string> tOutput;
    auto tLowerType = XMLGen::to_lower(aType);
    for(auto& tCriterion : aInputMetaData.criteria())
    {
        if(tLowerType == tCriterion.type())
        {
            tOutput.push_back(tCriterion.id());
        }
    }
    return tOutput;
}
// function get_criteria_ids

}