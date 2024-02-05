#include "UnitMain.hpp"

int main(int argc, char **argv)
{
    constexpr auto tNumRanksForTests = int{2};
    return Plato::Functional::parallel_unit_main(argc, argv, tNumRanksForTests);
}
