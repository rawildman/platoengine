#include "plato/test_utilities/UnitMain.hpp"

int main(int argc, char **argv)
{
    constexpr auto tNumRanksForTests = int { @NUM_RANKS_FOR_TEST@ };
    return plato::functional::test_utilities::parallel_unit_main(argc, argv, tNumRanksForTests);
}
