/*
 * Plato_AppCLI.hpp
 *
 *  Created on: Jan 28, 2022
 */

#include <mpi.h>

#include "Plato_Application.hpp"

namespace Plato
{

/******************************************************************************//**
 * \brief General Plato service Application (App) use to do system calls to other apps. 
**********************************************************************************/
class AppCLI : public Plato::Application
{
public:
    /******************************************************************************//**
     * \fn AppCLI
     * \brief Constrcutor for Command Line Interface (CLI) App.
     * \param [in] aArgc input argument count
     * \param [in] aArgv input argument vector
     * \param [in] aLocalComm local communicator
    **********************************************************************************/
    AppCLI(int aArgc, char **aArgv, MPI_Comm& aLocalComm);

    /******************************************************************************//**
     * \fn AppCLI
     * \brief Destructor for CLI App.
    **********************************************************************************/
    ~AppCLI();

    /******************************************************************************//**
     * \fn finalize
     * \brief Safely deallocate memory associated with the CLI App.
    **********************************************************************************/
    void finalize();

    /******************************************************************************//**
     * \fn initialize
     * \brief Safely allocate memory associated with the CLI App.
    **********************************************************************************/
    void initialize();

    /******************************************************************************//**
     * \fn compute
     * \brief Perform an operation associated with this app. Operations are responsible \n
              for running a set of code instructions to calculate a quantity of interests \n
              needed by another application in a Plato job. 
     * \param [in] aOperationName name of the operation to be performed, the name must be recognized by the CLI application
    **********************************************************************************/
    void compute(const std::string & aOperationName);

    /******************************************************************************//**
     * \fn exportData
     * \brief Export data needed by another application from the CLI application.
     * \param [in]  aArgumentName output data identifier/name recognized by the CLI application
     * \param [out] aExportData   Plato container used to save the CLI data to be exported from the CLI application
    **********************************************************************************/
    void exportData(const std::string & aArgumentName, Plato::SharedData & aExportData);

    /******************************************************************************//**
     * \fn importData
     * \brief Import data needed by the CLI application from another application.
     * \param [in]  aArgumentName import data identifier/name recognized by the CLI application
     * \param [out] aExportData   Plato container used to import the data needed by the CLI application
    **********************************************************************************/
    void importData(const std::string & aArgumentName, const Plato::SharedData & aImportData);

    /******************************************************************************//**
     * \fn exportDataMap 
     * \brief Export parallel graph needed by Plato to properly export data from the CLI application into another application. 
     * \param [in]  aDataLayout data layout (i.e. node-based or element-based data)
     * \param [out] aMyOwnedGlobalIDs global IDs owned by this processor
    **********************************************************************************/
    void exportDataMap(const Plato::data::layout_t & aDataLayout, std::vector<int> & aMyOwnedGlobalIDs);

private:
    AppCLI(const Plato::AppCLI& aRhs);
    Plato::AppCLI& operator=(const Plato::AppCLI& aRhs);
};
// class AppCLI

}

// namespace Plato