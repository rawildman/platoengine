#include <gtest/gtest.h>

#include <boost/fusion/adapted/struct/define_struct.hpp>

#include "GeometryInputBuilder.hpp"
// clang-format off
BOOST_FUSION_DEFINE_STRUCT((plato)(functional)(geometry)(library)(detail),
                           TestStructNoGeometryTypes,
                           (int, mInt)
                           (double, mDouble))

BOOST_FUSION_DEFINE_STRUCT((plato)(functional)(geometry)(library)(detail),
                           TestStructWithGeometryTypes,
                           (Plato::density_topology, mDensityTopology1)
                           (double, mDouble)
                           (Plato::brick_shape_geometry, mBrickShapeGeometry)
                           (boost::optional<Plato::density_topology>, mDensityTopology2))
// clang-format on
namespace plato::functional::geometry::library::unittest
{
TEST(GeometryInputBuilder, TupleIfGeometryInput)
{
    namespace pfgld = plato::functional::geometry::library::detail;
    {
        [[maybe_unused]] const std::tuple<> tEmptyTuple = pfgld::tuple_if_geometry_input<int>();
        EXPECT_EQ(std::tuple_size_v<decltype(tEmptyTuple)>, 0);
    }  // namespace plato::functional::geometry::library::detail;
    {
        [[maybe_unused]] const std::tuple<Plato::density_topology> tTuple =
            pfgld::tuple_if_geometry_input<Plato::density_topology>();
        EXPECT_EQ(std::tuple_size_v<decltype(tTuple)>, 1);
    }
}

TEST(GeometryInputBuilder, CatIfIsGeometryInput)
{
    namespace pfgld = plato::functional::geometry::library::detail;
    {
        auto tInitial = std::tuple<>{};
        constexpr std::size_t tNumFields = boost::fusion::result_of::size<detail::TestStructNoGeometryTypes>::value;
        [[maybe_unused]] const std::tuple<> tEmptyTuple =
            pfgld::cat_if_is_geometry_input<detail::TestStructNoGeometryTypes, tNumFields - 1>(tInitial);
        EXPECT_EQ(std::tuple_size_v<decltype(tEmptyTuple)>, 0);
    }  // namespace plato::functional::geometry::library::detail;
    {
        auto tInitial = std::tuple<>{};
        constexpr std::size_t tNumFields = boost::fusion::result_of::size<detail::TestStructWithGeometryTypes>::value;
        [[maybe_unused]] const auto tResultTuple =
            pfgld::cat_if_is_geometry_input<detail::TestStructWithGeometryTypes, tNumFields - 1>(tInitial);
        EXPECT_EQ(std::tuple_size_v<decltype(tResultTuple)>, 3);
    }
}

TEST(GeometryInputBuilder, MakeGeometryInputTuple)
{
    namespace pfgld = plato::functional::geometry::library::detail;
    {
        [[maybe_unused]] const auto tResult = pfgld::make_geometry_input_tuple{}(detail::TestStructNoGeometryTypes{});
        EXPECT_EQ(std::tuple_size_v<decltype(tResult)>, 0);
    }  // namespace plato::functional::geometry::library::detail;
    {
        [[maybe_unused]] const auto tResult = pfgld::make_geometry_input_tuple{}(detail::TestStructWithGeometryTypes{});
        EXPECT_EQ(std::tuple_size_v<decltype(tResult)>, 3);
        using ResultType = decltype(tResult);
        static_assert(std::is_same_v<std::decay_t<std::tuple_element_t<0, ResultType>>, Plato::density_topology>);
        static_assert(std::is_same_v<std::decay_t<std::tuple_element_t<1, ResultType>>, Plato::brick_shape_geometry>);
        static_assert(std::is_same_v<std::decay_t<std::tuple_element_t<2, ResultType>>, Plato::density_topology>);
    }
}

TEST(GeometryInputBuilder, VariantFromTuple)
{
    namespace pfgld = plato::functional::geometry::library::detail;
    using TupleType = std::tuple<int, double, char>;
    using VariantType = typename pfgld::VariantFromTuple<TupleType>::type;
    static_assert(std::is_same_v<VariantType, std::variant<int, double, char>>);
}

TEST(GeometryInputBuilder, GeometryInputVariant)
{
    namespace pfgld = plato::functional::geometry::library::detail;
    using TestInput = pfgld::GeometryInputVariant<pfgld::TestStructWithGeometryTypes>;
    using ExpectedType = std::variant<Plato::density_topology, Plato::brick_shape_geometry, Plato::density_topology>;
    static_assert(std::is_same_v<TestInput, ExpectedType>);
}

TEST(GeometryInputBuilder, GeometryInput)
{
    namespace pfgld = plato::functional::geometry::library::detail;
    using TestInput = pfgld::GeometryInputVariant<Plato::PlatoInput>;
    static_assert(std::variant_size_v<TestInput> == 2);
    static_assert(std::is_same_v<std::variant_alternative_t<0, TestInput>, Plato::density_topology>);
    static_assert(std::is_same_v<std::variant_alternative_t<1, TestInput>, Plato::brick_shape_geometry>);
}

TEST(GeometryInputBuilder, ValidatedGeometryInput)
{
    namespace pfgld = plato::functional::geometry::library::detail;
    using TestInput = pfgld::ValidatedGeometryInputVariant<Plato::PlatoInput>;
    static_assert(std::variant_size_v<TestInput> == 2);
    static_assert(std::is_same_v<std::variant_alternative_t<0, TestInput>,
                                 core::ValidatedInputTypeWrapper<Plato::density_topology>>);
    static_assert(std::is_same_v<std::variant_alternative_t<1, TestInput>,
                                 core::ValidatedInputTypeWrapper<Plato::brick_shape_geometry>>);
}
}  // namespace plato::functional::geometry::library::unittest