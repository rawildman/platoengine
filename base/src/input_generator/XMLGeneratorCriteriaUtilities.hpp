/*
 * XMLGeneratorCriteriaUtilities.hpp
 *
 *  Created on: March 24, 2022
 */

#pragma once

namespace XMLGen
{

struct InputData;

/******************************************************************************//**
 * \fn get_criteria_ids
 * \brief Return sequence container with all the criteria ids matching the requested criterion type. 
 * \param [in] aType criterion type
 * \param [in] aInputMetaData input metadata read from Plato input deck
**********************************************************************************/
std::vector<std::string> 
get_criteria_ids
(const std::string& aType,
 const XMLGen::InputData& aInputMetaData);

}
// namespace XMLGen