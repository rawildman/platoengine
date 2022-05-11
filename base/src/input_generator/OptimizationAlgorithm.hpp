/*
 *  OptimizationAlgorithm.hpp
 *
 *  Created on: May 9, 2022
 */
#pragma once

#include <string>
#include "pugixml.hpp"
#include "XMLGeneratorUtilities.hpp"
#include "Stage.hpp"

namespace PDir
{

class OptimizationAlgorithm
{
protected:
   using StagePtr = std::shared_ptr<Stage>;
   std::string mPackage;
   std::string mMaxIterations;

   void appendOutputStage(pugi::xml_node& aNode);
   void appendOptimizationVariables(pugi::xml_node& aNode,
                                    StagePtr aInitialization,
                                    StagePtr aUpperBound,
                                    StagePtr aLowerBound);
   void appendObjectiveData(pugi::xml_node& aNode,
                            StagePtr aObjectiveValue,
                            StagePtr aObjectiveGradient);
   void appendConstraintData(pugi::xml_node& aNode,
                             StagePtr aConstraintValue,
                             StagePtr aConstraintGradient);
   void appendBoundsData(pugi::xml_node& aNode);

public:
    OptimizationAlgorithm(const XMLGen::OptimizationParameters& aParameters);
    virtual void writeInterface(pugi::xml_node& aNode,
                                StagePtr aInitialization = nullptr,
                                StagePtr aUpperBound = nullptr,
                                StagePtr aLowerBound = nullptr,
                                StagePtr aObjectiveValue = nullptr,
                                StagePtr aObjectiveGradient = nullptr,
                                StagePtr aConstraintValue = nullptr,
                                StagePtr aConstraintdient = nullptr) = 0;
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
    OptimizationAlgorithmPlatoOC(const XMLGen::OptimizationParameters& aParameters);
    void writeInterface(pugi::xml_node& aNode,
                        StagePtr aInitialization = nullptr,
                        StagePtr aUpperBound = nullptr,
                        StagePtr aLowerBound = nullptr,
                        StagePtr aObjectiveValue = nullptr,
                        StagePtr aObjectiveGradient = nullptr,
                        StagePtr aConstraintValue = nullptr,
                        StagePtr aConstraintGradient = nullptr) override;
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
    OptimizationAlgorithmPlatoKSBC(const XMLGen::OptimizationParameters& aParameters);
    void writeInterface(pugi::xml_node& aNode,
                        StagePtr aInitialization = nullptr,
                        StagePtr aUpperBound = nullptr,
                        StagePtr aLowerBound = nullptr,
                        StagePtr aObjectiveValue = nullptr,
                        StagePtr aObjectiveGradient = nullptr,
                        StagePtr aConstraintValue = nullptr,
                        StagePtr aConstraintGradient = nullptr) override;
    void writeAuxiliaryFiles(pugi::xml_node& aNode) override;
};

class OptimizationAlgorithmPlatoKSAL : public OptimizationAlgorithm
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

    std::string mPenaltyParam;
    std::string mPenaltyParamScaleFactor;

public:
    OptimizationAlgorithmPlatoKSAL(const XMLGen::OptimizationParameters& aParameters);
    void writeInterface(pugi::xml_node& aNode,
                        StagePtr aInitialization = nullptr,
                        StagePtr aUpperBound = nullptr,
                        StagePtr aLowerBound = nullptr,
                        StagePtr aObjectiveValue = nullptr,
                        StagePtr aObjectiveGradient = nullptr,
                        StagePtr aConstraintValue = nullptr,
                        StagePtr aConstraintGradient = nullptr) override;
    void writeAuxiliaryFiles(pugi::xml_node& aNode) override;
};

class OptimizationAlgorithmPlatoMMA : public OptimizationAlgorithm
{
private:
     std::string mMaxNumOuterIterations;
     std::string mMoveLimit;
     std::string mAsymptoteExpansion;
     std::string mAsymptoteContraction;
     std::string mMaxNumSubProblemIter;
     std::string mControlStagnationTolerance;
     std::string mObjectiveStagnationTolerance;
     std::string mOutputSubProblemDiagnostics;
     std::string mSubProblemInitialPenalty;
     std::string mSubProblemPenaltyMultiplier;
     std::string mSubProblemFeasibilityTolerance;
     std::string mUpdateFrequency;
     std::string mUseIpoptForMMASubproblem;

public:
    OptimizationAlgorithmPlatoMMA(const XMLGen::OptimizationParameters& aParameters);
    void writeInterface(pugi::xml_node& aNode,
                        StagePtr aInitialization = nullptr,
                        StagePtr aUpperBound = nullptr,
                        StagePtr aLowerBound = nullptr,
                        StagePtr aObjectiveValue = nullptr,
                        StagePtr aObjectiveGradient = nullptr,
                        StagePtr aConstraintValue = nullptr,
                        StagePtr aConstraintGradient = nullptr) override;
    void writeAuxiliaryFiles(pugi::xml_node& aNode) override;
};


// class OptimizationAlgorithmROLLC : public OptimizationAlgorithm
// {
// private:

// public:
//     OptimizationAlgorithmROLLC(const OptimizationParameters& aParameters);
//     void writeInterface(pugi::xml_node& aNode,
//                         StagePtr aInitialization = nullptr,
//                         StagePtr aUpperBound = nullptr,
//                         StagePtr aLowerBound = nullptr) override;
//     void writeAuxiliaryFiles(pugi::xml_node& aNode) override;
// };

// class OptimizationAlgorithmROLBC : public OptimizationAlgorithm
// {
// private:

// public:
//     OptimizationAlgorithmROLBC(const OptimizationParameters& aParameters);
//     void writeInterface(pugi::xml_node& aNode,
//                         StagePtr aInitialization = nullptr,
//                         StagePtr aUpperBound = nullptr,
//                         StagePtr aLowerBound = nullptr) override;
//     void writeAuxiliaryFiles(pugi::xml_node& aNode) override;
// };

// class OptimizationAlgorithmROLAL : public OptimizationAlgorithm
// {
// private:

// public:
//     OptimizationAlgorithmROLAL(const OptimizationParameters& aParameters);
//     void writeInterface(pugi::xml_node& aNode,
//                         StagePtr aInitialization = nullptr,
//                         StagePtr aUpperBound = nullptr,
//                         StagePtr aLowerBound = nullptr) override;
//     void writeAuxiliaryFiles(pugi::xml_node& aNode) override;
// };

// class OptimizationAlgorithmDakota : public OptimizationAlgorithm
// {
// private:

// public:
//     OptimizationAlgorithmDakota(const OptimizationParameters& aParameters);
//     void writeInterface(pugi::xml_node& aNode,
//                         StagePtr aInitialization = nullptr,
//                         StagePtr aUpperBound = nullptr,
//                         StagePtr aLowerBound = nullptr) override;
//     void writeAuxiliaryFiles(pugi::xml_node& aNode) override;
// };

} //namespace