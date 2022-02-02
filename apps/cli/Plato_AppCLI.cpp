/*
 * Plato_AppCLI.cpp
 *
 *  Created on: Jan 28, 2022
 */

 #include "Plato_AppCLI.hpp"

namespace Plato
{

AppCLI::AppCLI(int aArgc, char **aArgv, MPI_Comm& aLocalComm){}

AppCLI::~AppCLI(){}

void AppCLI::finalize(){}

void AppCLI::initialize(){}

void AppCLI::compute(const std::string & aOperationName)
{}

void AppCLI::exportData(const std::string & aArgumentName, Plato::SharedData & aExportData){}

void AppCLI::importData(const std::string & aArgumentName, const Plato::SharedData & aImportData){}

void AppCLI::exportDataMap(const Plato::data::layout_t & aDataLayout, std::vector<int> & aMyOwnedGlobalIDs){}

}
// namespace Plato