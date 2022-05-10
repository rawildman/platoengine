/*
 *  OptimizationAlgorithm.hpp
 *
 *  Created on: May 9, 2022
 */
#pragma once

#include <string>
#include "pugixml.hpp"
#include "XMLGeneratorUtilities.hpp"

namespace XMLGen
{

class OptimizationAlgorithm
{
protected:
   std::string mPackage;
   std::string mMaxIterations;

public:
    OptimizationAlgorithm(const OptimizationParameters& aParameters);
    virtual void writeInterface(pugi::xml_node& aNode) = 0;
    virtual void writeAuxiliaryFiles(pugi::xml_node& aNode) = 0;
};

class OptimizationAlgorithmPlatoOC : public OptimizationAlgorithm
{
private:
    std::string mControlStagnationTolerance;
    std::string mObjectiveStagnationTolerance;
    std::string mGradientTolerance;
    std::string mProblemUpdateFrequency;

public:
    OptimizationAlgorithmPlatoOC(const OptimizationParameters& aParameters);
    void writeInterface(pugi::xml_node& aNode) override;
    void writeAuxiliaryFiles(pugi::xml_node& aNode) override;
};

class OptimizationAlgorithmPlatoKSBC : public OptimizationAlgorithm
{
private:
    std::string mMaxNumOuterIterations;
    std::string mTrustRegionExpansionFactor;
    std::string mTrustRegionContractionFactor;
    std::string mMaxTrustRegionIterations;

    std::string mInitialRadiusScale;
    std::string mMaxRadiusScale;
    std::string mHessianType;
    std::string mMinTrustRegionRadius;
    std::string mLimitedMemoryStorage;
    
    std::string mOuterGradientTolerance;
    std::string mOuterStationarityTolerance;
    std::string mOuterStagnationTolerance;
    std::string mOuterControlStagnationTolerance;
    std::string mOuterActualReductionTolerance;
    
    std::string mProblemUpdateFrequency;
    std::string mDisablePostSmoothing;
    std::string mTrustRegionRatioLow;
    std::string mTrustRegionRatioMid;
    std::string mTrustRegionRatioUpper;
      
public:
    OptimizationAlgorithmPlatoKSBC(const OptimizationParameters& aParameters);
    void writeInterface(pugi::xml_node& aNode) override;
    void writeAuxiliaryFiles(pugi::xml_node& aNode) override;
};

class OptimizationAlgorithmROL : public OptimizationAlgorithm
{
private:

public:
    OptimizationAlgorithmROL(const OptimizationParameters& aParameters);
    void writeInterface(pugi::xml_node& aNode) override;
    void writeAuxiliaryFiles(pugi::xml_node& aNode) override;
};

class OptimizationAlgorithmDakota : public OptimizationAlgorithm
{
private:

public:
    OptimizationAlgorithmDakota(const OptimizationParameters& aParameters);
    void writeInterface(pugi::xml_node& aNode) override;
    void writeAuxiliaryFiles(pugi::xml_node& aNode) override;
};



} //namespace