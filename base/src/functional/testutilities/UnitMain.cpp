#include "UnitMain.hpp"

#include <gtest/gtest.h>
#include <mpi.h>

#include <Kokkos_Core.hpp>

namespace Plato::Functional
{
int unit_main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    Kokkos::initialize(argc, argv);

    testing::InitGoogleTest(&argc, argv);
    const int returnVal = RUN_ALL_TESTS();

    Kokkos::finalize();
    MPI_Finalize();

    return returnVal;
}
}  // namespace Plato::Functional
