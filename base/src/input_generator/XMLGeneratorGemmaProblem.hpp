/*
 * XMLGeneratorGemmaProblem.hpp
 *
 *  Created on: March 25, 2022
 */

#pragma once

#include <vector>
#include "XMLGeneratorOperation.hpp"
#include "XMLGeneratorPerformer.hpp"
#include "XMLGeneratorSharedData.hpp"
#include "XMLGeneratorStage.hpp"
#include "XMLGeneratorUtilities.hpp"

namespace XMLGen
{

class XMLGeneratorProblem
{
protected:
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> mPerformerMain;
    std::shared_ptr<XMLGeneratorPerformer> mPerformer;

    std::vector<std::shared_ptr<XMLGeneratorOperation>> mOperations;
    std::vector<XMLGeneratorStage> mStages;
    
    std::shared_ptr<XMLGeneratorStage> mDakotaStage;

    std::vector<std::shared_ptr<XMLGeneratorSharedData>> mSharedData;

    std::string mInterfaceFileName;
    std::string mOperationsFileName;
    std::string mDefinesFileName;
    std::string mInputDeckName;
    std::string mMPISourceName;
    std::string mVerbose;

public:
    XMLGeneratorProblem();
    virtual void write_plato_main(pugi::xml_document& aDocument) = 0;
    virtual void write_plato_main_input(pugi::xml_document& aDocument) = 0;
    virtual void write_interface(pugi::xml_document& aDocument) = 0;
    virtual void write_mpisource(std::string aFileName) = 0;
    virtual void write_defines() = 0;


};

class XMLGeneratorGemmaProblem : public XMLGeneratorProblem 
{
private:
    // void addOperationsAndStages(pugi::xml_document& aDocument) override;
    int mNumParams;

public:
    XMLGeneratorGemmaProblem(const InputData& aMetaData);
    void write_plato_main(pugi::xml_document& aDocument) override;
    void write_plato_main_input(pugi::xml_document& aDocument) override;
    void write_interface(pugi::xml_document& aDocument) override;
    void write_mpisource(std::string aFileName) override;
    void write_defines() override;
    void create_evaluation_subdirectories_and_gemma_input(const InputData& aMetaData);
    void create_matched_power_balance_input_deck(const InputData& aMetaData);
};

}