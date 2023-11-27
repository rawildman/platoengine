#include <gtest/gtest.h>

#include <boost/fusion/adapted/struct/define_struct.hpp>

#include "detail/GeometryInputBuilder.hpp"
// clang-format off
#include "Plato_SuppressBoostNvccWarnings.hpp"
BOOST_FUSION_DEFINE_STRUCT((Plato)(Functional)(GeometryFactory)(Detail),
                           TestStructNoGeometryTypes,
                           (int, mInt)
                           (double, mDouble))

BOOST_FUSION_DEFINE_STRUCT((Plato)(Functional)(GeometryFactory)(Detail),
                           TestStructWithGeometryTypes,
                           (Plato::density_topology, mDensityTopology1)
                           (double, mDouble)
                           (Plato::brick_shape_geometry, mBrickShapeGeometry)
                           (boost::optional<Plato::density_topology>, mDensityTopology2))
#include "Plato_RestoreBoostNvccWarnings.hpp"
// clang-format on

TEST(GeometryInputBuilder, TupleIfGeometryInput)
{
    namespace pfgd = Plato::Functional::GeometryFactory::Detail;
    {
        [[maybe_unused]] const std::tuple<> tEmptyTuple = pfgd::tuple_if_geometry_input<int>();
        EXPECT_EQ(std::tuple_size_v<decltype(tEmptyTuple)>, 0);
    }  // namespace Plato::Functional::GeometryFactory::Detail;
    {
        [[maybe_unused]] const std::tuple<Plato::density_topology> tTuple =
            pfgd::tuple_if_geometry_input<Plato::density_topology>();
        EXPECT_EQ(std::tuple_size_v<decltype(tTuple)>, 1);
    }
}

TEST(GeometryInputBuilder, CatIfIsGeometryInput)
{
    namespace pfgd = Plato::Functional::GeometryFactory::Detail;
    {
        auto tInitial = std::tuple<>{};
        constexpr std::size_t tNumFields = boost::fusion::result_of::size<pfgd::TestStructNoGeometryTypes>::value;
        [[maybe_unused]] const std::tuple<> tEmptyTuple =
            pfgd::cat_if_is_geometry_input<pfgd::TestStructNoGeometryTypes, tNumFields - 1>(tInitial);
        EXPECT_EQ(std::tuple_size_v<decltype(tEmptyTuple)>, 0);
    }  // namespace Plato::Functional::GeometryFactory::Detail;
    {
        auto tInitial = std::tuple<>{};
        constexpr std::size_t tNumFields = boost::fusion::result_of::size<pfgd::TestStructWithGeometryTypes>::value;
        [[maybe_unused]] const auto tResultTuple =
            pfgd::cat_if_is_geometry_input<pfgd::TestStructWithGeometryTypes, tNumFields - 1>(tInitial);
        EXPECT_EQ(std::tuple_size_v<decltype(tResultTuple)>, 3);
    }
}

TEST(GeometryInputBuilder, MakeGeometryInputTuple)
{
    namespace pfgd = Plato::Functional::GeometryFactory::Detail;
    {
        [[maybe_unused]] const auto tResult = pfgd::make_geometry_input_tuple{}(pfgd::TestStructNoGeometryTypes{});
        EXPECT_EQ(std::tuple_size_v<decltype(tResult)>, 0);
    }  // namespace Plato::Functional::GeometryFactory::Detail;
    {
        [[maybe_unused]] const auto tResult = pfgd::make_geometry_input_tuple{}(pfgd::TestStructWithGeometryTypes{});
        EXPECT_EQ(std::tuple_size_v<decltype(tResult)>, 3);
        using ResultType = decltype(tResult);
        static_assert(std::is_same_v<std::decay_t<std::tuple_element_t<0, ResultType>>, Plato::density_topology>);
        static_assert(std::is_same_v<std::decay_t<std::tuple_element_t<1, ResultType>>, Plato::brick_shape_geometry>);
        static_assert(std::is_same_v<std::decay_t<std::tuple_element_t<2, ResultType>>, Plato::density_topology>);
    }
}

TEST(GeometryInputBuilder, VariantFromTuple)
{
    namespace pfgd = Plato::Functional::GeometryFactory::Detail;
    using TupleType = std::tuple<int, double, char>;
    using VariantType = typename pfgd::VariantFromTuple<TupleType>::type;
    static_assert(std::is_same_v<VariantType, std::variant<int, double, char>>);
}

TEST(GeometryInputBuilder, GeometryInputVariant)
{
    namespace pfgd = Plato::Functional::GeometryFactory::Detail;
    using TestInput = pfgd::GeometryInputVariant<pfgd::TestStructWithGeometryTypes>;
    using ExpectedType = std::variant<Plato::density_topology, Plato::brick_shape_geometry, Plato::density_topology>;
    static_assert(std::is_same_v<TestInput, ExpectedType>);
}

TEST(GeometryInputBuilder, GeometryInput)
{
    namespace pfgd = Plato::Functional::GeometryFactory::Detail;
    using TestInput = pfgd::GeometryInputVariant<Plato::PlatoInput>;
    static_assert(std::variant_size_v<TestInput> == 2);
    static_assert(std::is_same_v<std::variant_alternative_t<0, TestInput>, Plato::density_topology>);
    static_assert(std::is_same_v<std::variant_alternative_t<1, TestInput>, Plato::brick_shape_geometry>);
}
