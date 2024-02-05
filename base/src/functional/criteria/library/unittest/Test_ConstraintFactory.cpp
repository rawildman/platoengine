#include <gtest/gtest.h>

#include "ConstraintFactory.hpp"

namespace plato::functional::criteria::library::unittest
{
TEST(ConstraintFactory, DualVector)
{
    std::unique_ptr<ROL::StdVector<double>> tDualVector = plato::functional::criteria::library::make_dual_vector();
    ASSERT_EQ(tDualVector->dimension(), 1);
    EXPECT_EQ(tDualVector->getVector()->front(), 1.0);
}
}
