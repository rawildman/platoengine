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
    std::vector<XMLGeneratorOperation*> mOperations;
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

};

}