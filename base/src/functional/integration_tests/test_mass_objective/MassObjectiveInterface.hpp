#ifndef PLATO_FUNCTIONAL_MASSOBJECTIVEINTERFACE_H
#define PLATO_FUNCTIONAL_MASSOBJECTIVEINTERFACE_H

#include "CriterionInterface.hpp"

namespace plato::functional::integration_tests::test_mass_objective
{
class MassObjectiveInterface : public Plato::Functional::CriterionInterface
{
   public:
    ///@brief Construct a new Mass Objective Interface object
    MassObjectiveInterface();

    ///@brief required by the CriterionInterface, return the value of the criterion evaluated at the controls specified
    /// by the MeshProxy
    double value(const Plato::Functional::MeshProxy& aMeshProxy) const override;

    ///@brief required by the CriterionInterface, return the gradient of the criterion evaluated at the controls
    /// specified by the MeshProxy
    std::vector<double> gradient(const Plato::Functional::MeshProxy& aMeshProxy) const override;
};
}  // namespace plato::functional::integration_tests::test_mass_objective

#endif
