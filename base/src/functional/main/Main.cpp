#include <mpi.h>

#include <Kokkos_Core.hpp>

#include <iostream>

#include "OptimizationProblem.hpp"

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    Kokkos::initialize(argc, argv);

    if (argc == 2)
    {
        Plato::Functional::OptimizationProblem tOptimizationProblem(argv[1]);
        tOptimizationProblem.optimize();
    }
    else
    {
        std::cout << "Executable expects an input file name as an argument. Aborting." << std::endl;
    }

    Kokkos::finalize();
    MPI_Finalize();

    return 0;
}
