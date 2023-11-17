#include <gtest/gtest.h>

#include <cmath>
#include <filesystem>
#include <numeric>
#include <stk_util/parallel/Parallel.hpp>
#include <string_view>

#include "BrickShapeGeometry.hpp"
#include "JacobianColumnEvaluator.hpp"
#include "MeshProxy.hpp"
#include "STKUtilities.hpp"

TEST(Brick, CenterAndDims)
{
    namespace pf = Plato::Functional;
    ASSERT_EQ(stk::parallel_machine_size(MPI_COMM_WORLD), 1);

    constexpr std::string_view tFileName = "test.exo";
    constexpr pf::BrickDesign tDesignParameters = {1.0, -2.0, 3.0, 1.80, 3.90, 6.0};

    {
        constexpr double tDiscretizationSize = 1.0;
        auto mesh = pf::detail::create_mesh(tDesignParameters, tDiscretizationSize);
        pf::write_mesh(tFileName, mesh);
        constexpr unsigned tExpectedNumElements = 2 * 4 * 6;
        EXPECT_EQ(tExpectedNumElements, pf::element_size(tFileName));
    }
    {
        auto mesh = pf::detail::create_mesh(tDesignParameters);
        pf::write_mesh(tFileName, mesh);
        constexpr unsigned tExpectedNumElements = 1;
        EXPECT_EQ(tExpectedNumElements, pf::element_size(tFileName));
    }

    EXPECT_TRUE(std::filesystem::exists(tFileName));
    EXPECT_TRUE(std::filesystem::remove(tFileName));
}

TEST(Brick, SensitivityCenterX)
{
    namespace pf = Plato::Functional;
    constexpr int tCenterXIndex = 0;
    const auto tSensitivities = pf::detail::sensitivities(tCenterXIndex);
    const std::vector<double> tGold = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0};
    EXPECT_EQ(tSensitivities.size(), tGold.size());
    EXPECT_EQ(tSensitivities, tGold);
}

TEST(Brick, SensitivityCenterY)
{
    namespace pf = Plato::Functional;
    constexpr int tCenterYIndex = 1;
    const auto tSensitivities = pf::detail::sensitivities(tCenterYIndex);
    const std::vector<double> tGold = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0};
    EXPECT_EQ(tSensitivities.size(), tGold.size());
    EXPECT_EQ(tSensitivities, tGold);
}

TEST(Brick, SensitivityCenterZ)
{
    namespace pf = Plato::Functional;
    constexpr int tCenterZIndex = 2;
    const auto tSensitivities = pf::detail::sensitivities(tCenterZIndex);
    const std::vector<double> tGold = {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1};
    EXPECT_EQ(tSensitivities.size(), tGold.size());
    EXPECT_EQ(tSensitivities, tGold);
}

TEST(Brick, SensitivityLengthX)
{
    // Nodes 1,3,5,7 should be negative (alternating x)
    namespace pf = Plato::Functional;
    constexpr int tLengthXIndex = 3;
    const auto tSensitivities = pf::detail::sensitivities(tLengthXIndex);
    const std::vector<double> tGold = {-0.5, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0.5, 0, 0,
                                       -0.5, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0.5, 0, 0};
    EXPECT_EQ(tSensitivities.size(), tGold.size());
    EXPECT_EQ(tSensitivities, tGold);
}

TEST(Brick, SensitivityLengthY)
{
    // Nodes 1,2,5,6 should be negative
    // Nodes 3,4,7,8 positive
    namespace pf = Plato::Functional;
    constexpr int tLengthYIndex = 4;
    const auto tSensitivities = pf::detail::sensitivities(tLengthYIndex);
    const std::vector<double> tGold = {0, -0.5, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0.5, 0,
                                       0, -0.5, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0.5, 0};
    EXPECT_EQ(tSensitivities.size(), tGold.size());
    EXPECT_EQ(tSensitivities, tGold);
}

TEST(Brick, SensitivityLengthZ)
{
    namespace pf = Plato::Functional;
    // Nodes 1,2,3,4 should be negative
    // Nodes 5,6,7,8 positive
    constexpr int tLengthZIndex = 5;
    const auto tSensitivities = pf::detail::sensitivities(tLengthZIndex);
    const std::vector<double> tGold = {0, 0, -0.5, 0, 0, -0.5, 0, 0, -0.5, 0, 0, -0.5,
                                       0, 0, 0.5,  0, 0, 0.5,  0, 0, 0.5,  0, 0, 0.5};
    EXPECT_EQ(tSensitivities.size(), tGold.size());
    EXPECT_EQ(tSensitivities, tGold);
}

TEST(BrickSensitivities, JacobianEvaluator)
{
    namespace pf = Plato::Functional;
    const pf::JacobianColumnEvaluator tJacobian = {
        /*.mColumns=*/6,
        /*.mX=*/ROL::StdVector<double>{1.0, 2.0, 3.0, 4.0, 5.0, 6.0},
        /*.mColumnFunction=*/[](unsigned int i, ROL::StdVector<double>) {
            return ROL::StdVector<double>(ROL::makePtr<std::vector<double>>(pf::detail::sensitivities(i)));
        }};

    std::vector<double> tVec(24, 0.0);
    std::iota(tVec.begin(), tVec.end(), 1.0);
    const ROL::StdVector<double> tRolvec(ROL::makePtr<std::vector<double>>(tVec));

    const std::vector<double> tGold{92, 100, 108, 6, 12, 24};
    const ROL::StdVector<double> tRes = tRolvec * tJacobian;
    EXPECT_EQ(*(tRes.getVector()), tGold);
}

TEST(Brick, ABrick)
{
    namespace pf = Plato::Functional;
    const std::string tFileName = "brick.exo";

    constexpr pf::BrickDesign tDesignParameters = {/*.center_x = */ 1,
                                                   /*.center_y = */ -2,
                                                   /*.center_z = */ -3,
                                                   /*.dimension_x = */ 2,
                                                   /*.dimension_y = */ 4,
                                                   /*.dimension_z = */ 6};

    constexpr double tDiscretizationSize = 1.0;
    pf::BrickShapeGeometry tBrick(tFileName, tDiscretizationSize);

    const pf::MeshProxy tMP = tBrick.generateMesh(tDesignParameters);
    EXPECT_EQ(tMP.mFileName, tFileName);

    constexpr unsigned tExpectedNumElements = 2 * 4 * 6;
    EXPECT_EQ(tExpectedNumElements, pf::element_size(tFileName));

    EXPECT_TRUE(std::filesystem::exists(tFileName));
    EXPECT_TRUE(std::filesystem::remove(tFileName));
}

TEST(Brick, ConvertDesignParametersToROLStdVector)
{
    namespace pf = Plato::Functional;
    constexpr pf::BrickDesign tDesignParameters = {/*.center_x = */ 1,
                                                   /*.center_y = */ -2,
                                                   /*.center_z = */ -3,
                                                   /*.dimension_x = */ 2,
                                                   /*.dimension_y = */ 4,
                                                   /*.dimension_z = */ 6};
    const ROL::StdVector<double> tResult = pf::detail::to_rol_std_vector(tDesignParameters);

    const std::vector<double> tGold{tDesignParameters.center_x,    tDesignParameters.center_y,
                                    tDesignParameters.center_z,    tDesignParameters.dimension_x,
                                    tDesignParameters.dimension_y, tDesignParameters.dimension_z};

    EXPECT_EQ(*(tResult.getVector()), tGold);
}

TEST(Brick, Jacobian)
{
    namespace pf = Plato::Functional;
    const std::string tFileName = "brick.exo";

    constexpr pf::BrickDesign tDesignParameters = {/*.center_x = */ 1,
                                                   /*.center_y = */ -2,
                                                   /*.center_z = */ -3,
                                                   /*.dimension_x = */ 2,
                                                   /*.dimension_y = */ 4,
                                                   /*.dimension_z = */ 6};

    const pf::BrickShapeGeometry tBrick(tFileName);
    const pf::JacobianColumnEvaluator tJacobian = tBrick.jacobian(tDesignParameters);

    constexpr unsigned int tNumNodes = 8;
    constexpr unsigned int tNumCoordinates = 3;
    std::vector<double> tVec(tNumNodes * tNumCoordinates, 0.0);
    std::iota(tVec.begin(), tVec.end(), 1.0);
    const ROL::StdVector<double> tRolvec(ROL::makePtr<std::vector<double>>(tVec));

    const std::vector<double> tGold{92, 100, 108, 6, 12, 24};
    const ROL::StdVector<double> tRes = tRolvec * tJacobian;
    EXPECT_EQ(*(tRes.getVector()), tGold);
}

TEST(Brick, ToROLStdVector)
{
    namespace pf = Plato::Functional;
    constexpr pf::BrickDesign tDesignParameters = {/*.center_x = */ 1,
                                                   /*.center_y = */ -2,
                                                   /*.center_z = */ -3,
                                                   /*.dimension_x = */ 2,
                                                   /*.dimension_y = */ 4,
                                                   /*.dimension_z = */ 6};

    const ROL::StdVector<double> tAsROLVector = pf::detail::to_rol_std_vector(tDesignParameters);
    EXPECT_EQ(tDesignParameters.center_x, tAsROLVector.getVector()->at(0));
    EXPECT_EQ(tDesignParameters.center_y, tAsROLVector.getVector()->at(1));
    EXPECT_EQ(tDesignParameters.center_z, tAsROLVector.getVector()->at(2));
    EXPECT_EQ(tDesignParameters.dimension_x, tAsROLVector.getVector()->at(3));
    EXPECT_EQ(tDesignParameters.dimension_y, tAsROLVector.getVector()->at(4));
    EXPECT_EQ(tDesignParameters.dimension_z, tAsROLVector.getVector()->at(5));

    const std::unique_ptr<ROL::StdVector<double>> tAsROLVectorPtr =
        pf::detail::to_rol_std_vector_ptr(tDesignParameters);
    EXPECT_EQ(tDesignParameters.center_x, tAsROLVectorPtr->getVector()->at(0));
    EXPECT_EQ(tDesignParameters.center_y, tAsROLVectorPtr->getVector()->at(1));
    EXPECT_EQ(tDesignParameters.center_z, tAsROLVectorPtr->getVector()->at(2));
    EXPECT_EQ(tDesignParameters.dimension_x, tAsROLVectorPtr->getVector()->at(3));
    EXPECT_EQ(tDesignParameters.dimension_y, tAsROLVectorPtr->getVector()->at(4));
    EXPECT_EQ(tDesignParameters.dimension_z, tAsROLVectorPtr->getVector()->at(5));
}
