/*
 OptimizationAlgorithmFactory.hpp
 *
 *  Created on: May 9, 2022
 */
#pragma once

#include <string>
#include <memory>
#include "XMLGeneratorUtilities.hpp"
#include "OptimizationAlgorithm.hpp"

namespace PDir
{

class OptimizationAlgorithmFactory
{
public:
    OptimizationAlgorithmFactory();
    std::shared_ptr<OptimizationAlgorithm> create(const XMLGen::InputData& aMetaData);

};


}