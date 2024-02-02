#ifndef PLATO_FUNCTIONAL_MASSOBJECTIVEINTERFACE_H
#define PLATO_FUNCTIONAL_MASSOBJECTIVEINTERFACE_H

#include "CriterionInterface.hpp"

namespace Plato::Functional
{
class MassObjectiveInterface : public CriterionInterface
{
   public:
    ///@brief Construct a new Mass Objective Interface object
    ///
    MassObjectiveInterface();

    ///@brief required by the CriterionInterface, return the value of the criterion evaluated at the controls specified
    /// by the MeshProxy
    ///
    ///@param aMeshProxy
    ///@return double
    double value(const MeshProxy& aMeshProxy) const override;

    ///@brief required by the CriterionInterface, return the gradient of the criterion evaluated at the controls
    /// specified by the MeshProxy
    ///
    ///@param aMeshProxy
    ///@return std::vector<double>
    std::vector<double> gradient(const MeshProxy& aMeshProxy) const override;
};
}  // namespace Plato::Functional

#endif
