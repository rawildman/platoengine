#include <gtest/gtest.h>

#include "ConstraintFactory.hpp"

TEST(ConstraintFactory, DualVector)
{
    std::unique_ptr<ROL::StdVector<double>> tDualVector = Plato::Functional::ConstraintFactory::make_dual_vector();
    ASSERT_EQ(tDualVector->dimension(), 1);
    EXPECT_EQ(tDualVector->getVector()->front(), 1.0);
}
