#include "UnitMain.hpp"

#include <gtest/gtest.h>
#include <mpi.h>

#include <Kokkos_Core.hpp>

namespace Plato::Functional
{
/// Removes listeners on all but rank 0
void setup_listeners()
{
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank != 0)
    {
        ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
        delete listeners.Release(listeners.default_result_printer());
    }
}

int unit_main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    Kokkos::initialize(argc, argv);

    testing::InitGoogleTest(&argc, argv);
    setup_listeners();
    const int returnVal = RUN_ALL_TESTS();

    Kokkos::finalize();
    MPI_Finalize();

    return returnVal;
}
}  // namespace Plato::Functional
