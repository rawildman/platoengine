#ifndef PLATO_INTEGRATION_TESTS_TEST_MASS_OBJECTIVE_PARALLELMASSOBJECTIVEINTERFACE
#define PLATO_INTEGRATION_TESTS_TEST_MASS_OBJECTIVE_PARALLELMASSOBJECTIVEINTERFACE

#include <mpi.h>

#include <boost/mpi/communicator.hpp>

#include "plato/criteria/library/CriterionInterface.hpp"

namespace plato::integration_tests::test_mass_objective
{
class ParallelMassObjectiveInterface : public criteria::library::CriterionInterface
{
   public:
    ParallelMassObjectiveInterface(MPI_Comm aComm);

    ///@brief required by the CriterionInterface, return the value of the criterion evaluated at the controls specified
    /// by the MeshProxy
    [[nodiscard]] double value(const core::MeshProxy& aMeshProxy) const override;

    ///@brief required by the CriterionInterface, return the gradient of the criterion evaluated at the controls
    /// specified by the MeshProxy
    [[nodiscard]] std::vector<double> gradient(const core::MeshProxy& aMeshProxy) const override;

   private:
    boost::mpi::communicator mComm;
};
}  // namespace plato::integration_tests::test_mass_objective

#endif
