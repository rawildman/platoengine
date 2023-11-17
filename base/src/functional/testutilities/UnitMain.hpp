#ifndef PLATO_FUNCTIONAL_UNITMAIN
#define PLATO_FUNCTIONAL_UNITMAIN

namespace Plato::Functional
{
/// @brief A unit test main for gtest.
///
/// Call this from any gtest unit tester. It calls the appropriate
/// gtest functions as well as initializing MPI and Kokkos.
int unit_main(int argc, char **argv);
}

#endif
