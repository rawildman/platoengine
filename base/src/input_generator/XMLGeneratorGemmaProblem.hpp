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
#include "XMLGeneratorUtilities.hpp"

namespace XMLGen
{

class XMLGeneratorProblem
{
protected:
    std::vector<std::shared_ptr<XMLGeneratorOperation>> mOperations;
    std::vector<std::shared_ptr<XMLGeneratorPerformer>> mPerformers;
    std::vector<std::shared_ptr<XMLGeneratorSharedData>> mSharedData;

    void addPlatoServicesPerformers(int aStartID, int aEndID);

public:
    XMLGeneratorProblem();
    virtual void write_plato_main(pugi::xml_document& aDocument) = 0;
    virtual void write_interface(pugi::xml_document& aDocument) = 0;

};

class XMLGeneratorGemmaProblem : public XMLGeneratorProblem 
{
private:
    void addSharedDataPairs(int aNumParameters, int aEvaluations);

public:
    XMLGeneratorGemmaProblem(const InputData& aMetaData);
    void write_plato_main(pugi::xml_document& aDocument) override;
    void write_interface(pugi::xml_document& aDocument) override;
    void create_evaluation_subdirectories_and_gemma_input(const InputData& aMetaData);
    void create_matched_power_balance_input_deck(const InputData& aMetaData);
};

}