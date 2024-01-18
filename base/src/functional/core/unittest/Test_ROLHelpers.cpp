#include <gtest/gtest.h>

#include "ROLHelpers.hpp"

TEST(ROLHelpers, ToDynamicVector)
{
    namespace pf = Plato::Functional;
    const auto tROLStdVector = ROL::StdVector<double>{1.0, 2.0, 3.0};
    const ROL::StdVector<double>& tROLVector = tROLStdVector;
    const pf::Core::DynamicVector<double> tDynamicVector = pf::to_dynamic_vector(tROLVector);
    EXPECT_EQ(tDynamicVector.stdVector(), *tROLStdVector.getVector());
}

TEST(ROLHelpers, ToROLVector)
{
    namespace pf = Plato::Functional;
    const auto tDynamicVector = pf::Core::DynamicVector<double>{1.0, 2.0, 3.0};
    const ROL::StdVector<double> tROLStdVector = pf::to_rol_vector(tDynamicVector);
    EXPECT_EQ(tDynamicVector.stdVector(), *tROLStdVector.getVector());
}

TEST(ROLHelpers, MakeROLVector)
{
    namespace pf = Plato::Functional;
    const auto tDynamicVector = pf::Core::DynamicVector<double>{1.0, 2.0, 3.0};
    const ROL::Ptr<ROL::Vector<double>> tROLVector = pf::make_rol_vector(tDynamicVector);
    const auto& tROLStdVector = dynamic_cast<const ROL::StdVector<double>&>(*tROLVector);
    EXPECT_EQ(tDynamicVector.stdVector(), *tROLStdVector.getVector());
}

TEST(ROLHelpers, AssignVector)
{
    namespace pf = Plato::Functional;
    const auto tVector = std::vector{1.0, 2.0, 3.0};
    auto tROLStdVector = ROL::StdVector<double>{-1.0, -2.0};
    pf::assign_vector(tROLStdVector, tVector);
    EXPECT_EQ(*tROLStdVector.getVector(), tVector);
}
