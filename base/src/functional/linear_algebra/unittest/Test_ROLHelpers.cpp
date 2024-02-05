#include <gtest/gtest.h>

#include "ROLHelpers.hpp"

namespace plato::functional::linear_algebra::unittest
{
TEST(ROLHelpers, ToDynamicVector)
{
    const auto tROLStdVector = ROL::StdVector<double>{1.0, 2.0, 3.0};
    const ROL::StdVector<double>& tROLVector = tROLStdVector;
    const DynamicVector<double> tDynamicVector = to_dynamic_vector(tROLVector);
    EXPECT_EQ(tDynamicVector.stdVector(), *tROLStdVector.getVector());
}

TEST(ROLHelpers, ToROLVector)
{
    const auto tDynamicVector = DynamicVector<double>{1.0, 2.0, 3.0};
    const ROL::StdVector<double> tROLStdVector = to_rol_vector(tDynamicVector);
    EXPECT_EQ(tDynamicVector.stdVector(), *tROLStdVector.getVector());
}

TEST(ROLHelpers, MakeROLVector)
{
    const auto tDynamicVector = DynamicVector<double>{1.0, 2.0, 3.0};
    const ROL::Ptr<ROL::Vector<double>> tROLVector = make_rol_vector(tDynamicVector);
    const auto& tROLStdVector = dynamic_cast<const ROL::StdVector<double>&>(*tROLVector);
    EXPECT_EQ(tDynamicVector.stdVector(), *tROLStdVector.getVector());
}

TEST(ROLHelpers, AssignVector)
{
    const auto tVector = std::vector{1.0, 2.0, 3.0};
    auto tROLStdVector = ROL::StdVector<double>{-1.0, -2.0};
    assign_vector(tROLStdVector, tVector);
    EXPECT_EQ(*tROLStdVector.getVector(), tVector);
}
}  // namespace plato::functional::linear_algebra::unittest
