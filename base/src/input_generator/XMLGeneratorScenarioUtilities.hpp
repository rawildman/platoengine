/*
 * XMLGeneratorScenarioUtilities.hpp
 *
 *  Created on: Mar 23, 2022
 *
 */

#pragma once

namespace XMLGen
{

struct InputData;

/******************************************************************************//**
 * \fn get_unique_scenario_ids
 * \brief Return unique scenario identifiers associated with target code/app supported in Plato. 
 * \param [in] aTargetCodeName target code/app name
 * \param [in] aInputMetaData  input metadata read from Plato input deck
 * \return container of unique scenario ids
**********************************************************************************/
std::vector<std::string> 
get_unique_scenario_ids
(const std::string& aTargetCodeName,
 const XMLGen::InputData& aInputMetaData);

}
 // namespace XMLGen