/*
 * PlatoCLI_App_UnitTester.cpp
 *
 *  Created on: Feb 01, 2022
 */

#include <gtest/gtest.h>

#include <mpi.h>
#include <memory>
#include <unordered_map>

#include "Plato_Macros.hpp"
#include "Plato_Parser.hpp"
#include "Plato_Application.hpp"
#include "Plato_FreeFunctions.hpp"

namespace Plato
{

struct SupportedOperations
{
    std::vector<std::string> mOperations = {"systemcall"};
};

class LocalOperation
{
public:
    LocalOperation(Plato::Application* aLocalApp) : mLocalApp(aLocalApp) {}
    virtual ~LocalOperation(){}

    virtual void operator()() = 0;
    virtual std::string name() const = 0;

protected:
    Plato::Application* mLocalApp;
};

class SystemCall : public Plato::LocalOperation
{
public:
    SystemCall(Plato::Application* aLocalApp, Plato::InputData& aNode) : 
        Plato::LocalOperation(aLocalApp)
    {
        this->initialize(aNode);
    }
    virtual ~SystemCall(){}

    void operator()(){};
    std::string name() const { return "SystemCall"; }

private:
    void initialize(Plato::InputData& aNode)
    {
        mLaunchCommand = Plato::Get::String(aNode, "Command");
        for(auto tStrValue : aNode.getByName<std::string>("Argument"))
        {
            mArguments.push_back(tStrValue);
        }   
    }

    void setArgumentPointers(const std::vector<std::string> &aArguments, std::vector<char*>& aArgumentPointers)
    {
        for(const auto &tMyArgument : aArguments) 
        {
            const int tLengthWithNullTerminator = tMyArgument.length() + 1;
            char *tCharPointer = new char [tLengthWithNullTerminator];
            std::copy_n(tMyArgument.c_str(), tLengthWithNullTerminator, tCharPointer);
            aArgumentPointers.push_back(tCharPointer);
        }
        aArgumentPointers.push_back(nullptr);
    }

    void launch(const std::vector<std::string> &aArguments)
    {
        std::vector<char*> tArgumentPointers;
        this->setArgumentPointers(aArguments, tArgumentPointers);

        /*
        MPI_Comm tInterComm;
        auto tExitStatus = MPI_Comm_spawn(mLaunchCommand.c_str(), tArgumentPointers.data(), 1, MPI_INFO_NULL, 0, mLocalApp->getComm(), &tInterComm, MPI_ERRCODES_IGNORE);
        if (tExitStatus != MPI_SUCCESS)
        {
            std::string tErrorMessage = std::string("System call ' ") + mStringCommand + std::string(" 'exited with exit status: ") + std::to_string(tExitStatus);
            THROWERR(tErrorMessage)
        }
        */
    
        for(auto tCharPointer : tArgumentPointers) 
        {
            delete [] tCharPointer;
        }
    }

private:
    std::string mLaunchCommand;
    std::vector<std::string> mArguments;
};

/******************************************************************************//**
 * \brief General Plato service Application (App) use to do system calls to other apps. 
**********************************************************************************/
class CommandLineInterface : public Plato::Application
{
public:
    /******************************************************************************//**
     * \fn CommandLineInterface()
     * \brief Constrcutor for Command Line Interface (CLI) App.
     * \param [in] aArgc input argument count
     * \param [in] aArgv input argument vector
     * \param [in] aLocalComm local communicator
    **********************************************************************************/
    CommandLineInterface(int aArgc, char **aArgv, MPI_Comm& aLocalComm) : 
        mLocalComm(aLocalComm)
    {
        const char *tInputChar = getenv("PLATO_APP_FILE");
        auto tParser = std::make_shared<Plato::PugiParser>();
        if(tInputChar)
        {
            mInputFileData = tParser->parseFile(tInputChar);
        }
        else
        {
            THROWERR("PLATO_APP_FILE keyword is empty. An input file must be provided to the System Call App.")
        }
    }

    /******************************************************************************//**
     * \fn ~CommandLineInterface()
     * \brief Destructor for CLI App.
    **********************************************************************************/
    ~CommandLineInterface(){}

    /******************************************************************************//**
     * \fn finalize
     * \brief Safely deallocate memory associated with the CLI App.
    **********************************************************************************/
    void finalize(){}

    /******************************************************************************//**
     * \fn initialize
     * \brief Safely allocate memory associated with the CLI App.
    **********************************************************************************/
    void initialize(){}

    /******************************************************************************//**
     * \fn compute
     * \brief Perform an operation associated with this app. Operations are responsible \n
              for running a set of code instructions to calculate a quantity of interests \n
              needed by another application in a Plato job. 
     * \param [in] aOperationName name of the operation to be performed, the name must be recognized by the CLI application
    **********************************************************************************/
    void compute(const std::string & aOperationName){}

    /******************************************************************************//**
     * \fn exportData
     * \brief Export data needed by another application from the CLI application.
     * \param [in]  aArgumentName output data identifier/name recognized by the CLI application
     * \param [out] aExportData   Plato container used to save the CLI data to be exported from the CLI application
    **********************************************************************************/
    void exportData(const std::string & aArgumentName, Plato::SharedData & aExportData){}

    /******************************************************************************//**
     * \fn importData
     * \brief Import data needed by the CLI application from another application.
     * \param [in]  aArgumentName import data identifier/name recognized by the CLI application
     * \param [out] aExportData   Plato container used to import the data needed by the CLI application
    **********************************************************************************/
    void importData(const std::string & aArgumentName, const Plato::SharedData & aImportData){}

    /******************************************************************************//**
     * \fn exportDataMap 
     * \brief Export parallel graph needed by Plato to properly export data from the CLI application into another application. 
     * \param [in]  aDataLayout data layout (i.e. node-based or element-based data)
     * \param [out] aMyOwnedGlobalIDs global IDs owned by this processor
    **********************************************************************************/
    void exportDataMap(const Plato::data::layout_t & aDataLayout, std::vector<int> & aMyOwnedGlobalIDs){}

    /******************************************************************************//**
     * \fn getComm 
     * \brief Return a const reference to the local MPI communicator.
     * \return const reference to the local MPI communicator
    **********************************************************************************/    
    const MPI_Comm& getComm() const
    {
        return (mLocalComm);
    }

private:
    void isOperationSupported(const std::string& aName)
    {
        auto tItr = std::find(mSupported.mOperations.begin(), mSupported.mOperations.end(), aName);
        if(tItr == mSupported.mOperations.end())
        {
            auto tMsg = std::string("Requested operation '") + aName + "' is not supported by System Call App.\n";
            tMsg + "Supported Operations:\n";
            for(auto& tOpName : mSupported.mOperations)
            {
                tMsg + "  " + tOpName + "\n";
            }
            THROWERR(tMsg);
        }
    }

    void appendSystemCall(Plato::InputData& aNode)
    {
        auto aOperation = Plato::tolower(Plato::Get::String(aNode, "Function"));
        if(aOperation == "systemcall")
        {
            auto tName = Plato::Get::String(aNode, "Name");
            mNameToOperationMap[tName] = std::make_shared<Plato::SystemCall>(this, aNode);
        }
    }

    void setOperationsMap()
    {
        if(mInputFileData.empty())
        {
            THROWERR(std::string("Input application file with local app operations is empty. ") + 
                "The file provided in PLATO_APP_FILE was probably not parsed correctly by Plato.")
        }

        for (auto& tNode : mInputFileData.getByName<Plato::InputData>("Operation"))
        {
            // check if requested operation is supported
            auto tStrFunction = Plato::tolower(Plato::Get::String(tNode, "Function"));
            this->isOperationSupported(tStrFunction);

            // append local operations to map if requested
            this->appendSystemCall(tNode);
        }
    }

private:
    MPI_Comm mLocalComm;

    // input operations metadta
    Plato::InputData mInputFileData;

    Plato::SupportedOperations mSupported;

    // local map from operation name to function
    std::unordered_map<std::string, std::shared_ptr<Plato::LocalOperation>> mNameToOperationMap;

private:
    CommandLineInterface(const Plato::CommandLineInterface& aRhs);
    Plato::CommandLineInterface& operator=(const Plato::CommandLineInterface& aRhs);
};
// class CommandLineInterface

}

namespace PlatoTestAppCLI
{

TEST(PlatoTestAppCLI, Test)
{
}

}
// namespace PlatoTestAppCLI