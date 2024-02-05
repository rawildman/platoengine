#include "MassObjectiveInterface.hpp"

#include <mpi.h>

#include <iostream>

#include "MassObjective.hpp"

namespace plato::functional::integration_tests::test_mass_objective
{
MassObjectiveInterface::MassObjectiveInterface()
{
    int tMPIInitialized = 0;
    MPI_Initialized(&tMPIInitialized);
    if (tMPIInitialized == 0)
    {
        std::cout << "Initializing MPI in MassObjective" << std::endl;
        int tArgc = 0;
        char** tArgv = nullptr;
        MPI_Init(&tArgc, &tArgv);
    }
    else
    {
        std::cout << "MPI already initialized in MassObjective" << std::endl;
    }
}

double MassObjectiveInterface::value(const Plato::Functional::MeshProxy& aMeshProxy) const
{
    constexpr double tDensity = 1.0;
    constexpr double tTarget = 0.0;
    const auto tMassObjective = MassObjective{tDensity, tTarget};
    return tMassObjective.mass(aMeshProxy.mFileName.string());
}

std::vector<double> MassObjectiveInterface::gradient(const Plato::Functional::MeshProxy& aMeshProxy) const
{
    ///@todo Populate the gradient with actual values
    constexpr double tDensity = 1.0;
    constexpr double tTarget = 0.0;
    const auto tMassObjective = MassObjective{tDensity, tTarget};
    constexpr unsigned int tNumDimensions = 3;
    const unsigned int tGradientSize = tMassObjective.numMeshNodes(aMeshProxy.mFileName.string()) * tNumDimensions;
    return std::vector<double>(tGradientSize, 0.0);
}

std::unique_ptr<Plato::Functional::CriterionInterface> plato_create_criterion(const std::vector<std::string>&)
{
    return std::make_unique<MassObjectiveInterface>();
}
}  // namespace plato::functional::integration_tests::test_mass_objective
