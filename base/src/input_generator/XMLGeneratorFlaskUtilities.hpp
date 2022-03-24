/*
 * XMLGeneratorFlaskUtilities.hpp
 *
 *  Created on: March 24, 2022
 */

#pragma 

#include <string>

namespace XMLGen
{

struct OperationMetaData;

namespace flask
{

/******************************************************************************//**
 * \fn write_web_app_executable
 * \brief Write executable to launch a Plato web application.
 * \param [in]  aBaseFileName  base executable name
 * \param [in]  aFileExtension executable file extension
 * \param [out] aOperationMetaData operation metadata structure
**********************************************************************************/
void write_web_app_executable
(const std::string& aBaseFileName,
 const std::string& aFileExtension,
 XMLGen::OperationMetaData& aOperationMetaData);

}
// namespace flask

}
// namespace XMLGen