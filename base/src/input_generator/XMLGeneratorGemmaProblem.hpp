/*
 * XMLGeneratorGemmaProblem.hpp
 *
 *  Created on: March 25, 2022
 */

#pragma once

#include <vector>
#include "XMLGeneratorOperation.hpp"
#include "XMLGeneratorUtilities.hpp"

namespace XMLGen
{

class XMLGeneratorProblem
{
protected:
    std::vector<std::shared_ptr<XMLGeneratorOperation>> mOperations;
public:
    XMLGeneratorProblem();
    virtual void write_plato_main(pugi::xml_document& aDocument) = 0;

};

class XMLGeneratorGemmaProblem : public XMLGeneratorProblem 
{
private:

public:
    XMLGeneratorGemmaProblem(const InputData& aMetaData);
    void write_plato_main(pugi::xml_document& aDocument) override;
    void create_evaluation_subdirectories_and_gemma_input(const InputData& aMetaData);
    void create_matched_power_balance_input_deck(const InputData& aMetaData);
};

}