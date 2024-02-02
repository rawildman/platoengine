#include "UnitMain.hpp"

int main(int argc, char **argv)
{
    constexpr auto tNumRanksForTests = int{@NUM_RANKS_FOR_TEST@};
    return Plato::Functional::parallel_unit_main(argc, argv, tNumRanksForTests);
}
