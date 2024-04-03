#include <gtest/gtest.h>

#include <boost/fusion/adapted/struct/define_struct.hpp>

#include "plato/core/VariantInputBuilder.hpp"

// clang-format off
BOOST_FUSION_DEFINE_STRUCT((plato)(core)(unittest),
                           TestStructNoVariantTypes,
                           (int, mInt)
                           (double, mDouble))

BOOST_FUSION_DEFINE_STRUCT((plato)(core)(unittest),
                           TestStructWithVariantTypes,
                           (double, mDouble)
                           (bool, mBool)
                           (boost::optional<float>, mOptionalFloat))

template<typename T>
struct IsInTestVariant
{
    constexpr static bool value = false;
};

template<>
struct IsInTestVariant<bool>
{
    constexpr static bool value = true;
};

template<>
struct IsInTestVariant<float>
{
    constexpr static bool value = true;
};

// clang-format on
namespace plato::core::unittest
{
TEST(VariantInputBuilder, TupleIfVariantInput)
{
    {
        [[maybe_unused]] const std::tuple<> tEmptyTuple = tuple_if_variant_input<int, IsInTestVariant>();
        EXPECT_EQ(std::tuple_size_v<decltype(tEmptyTuple)>, 0u);
    }
    {
        [[maybe_unused]] const std::tuple<bool> tTuple = tuple_if_variant_input<bool, IsInTestVariant>();
        EXPECT_EQ(std::tuple_size_v<decltype(tTuple)>, 1u);
    }
}

TEST(VariantInputBuilder, CatIfIsVariantInput)
{
    {
        auto tInitial = std::tuple<>{};
        constexpr std::size_t tNumFields = boost::fusion::result_of::size<TestStructNoVariantTypes>::value;
        [[maybe_unused]] const std::tuple<> tEmptyTuple =
            cat_if_is_variant_input<TestStructNoVariantTypes, IsInTestVariant, tNumFields - 1>(tInitial);
        EXPECT_EQ(std::tuple_size_v<decltype(tEmptyTuple)>, 0u);
    }
    {
        auto tInitial = std::tuple<>{};
        constexpr std::size_t tNumFields = boost::fusion::result_of::size<TestStructWithVariantTypes>::value;
        [[maybe_unused]] const auto tResultTuple =
            cat_if_is_variant_input<TestStructWithVariantTypes, IsInTestVariant, tNumFields - 1>(tInitial);
        EXPECT_EQ(std::tuple_size_v<decltype(tResultTuple)>, 2u);
    }
}

TEST(VariantInputBuilder, MakeVariantInputTuple)
{
    {
        [[maybe_unused]] const auto tResult = make_variant_input_tuple<IsInTestVariant>{}(TestStructNoVariantTypes{});
        EXPECT_EQ(std::tuple_size_v<decltype(tResult)>, 0u);
    }
    {
        [[maybe_unused]] const auto tResult = make_variant_input_tuple<IsInTestVariant>{}(TestStructWithVariantTypes{});
        static_assert(std::tuple_size_v<decltype(tResult)> == 2u);
        using ResultType = decltype(tResult);
        static_assert(std::is_same_v<std::decay_t<std::tuple_element_t<0, ResultType>>, float>);
        static_assert(std::is_same_v<std::decay_t<std::tuple_element_t<1, ResultType>>, bool>);
    }
}

TEST(VariantInputBuilder, VariantFromTuple)
{
    using TupleType = std::tuple<int, double, char>;
    using VariantType = typename VariantFromTuple<TupleType>::type;
    static_assert(std::is_same_v<VariantType, std::variant<int, double, char>>);
}

TEST(VariantInputBuilder, InputVariant)
{
    using TestInput = InputVariant<TestStructWithVariantTypes, IsInTestVariant>;
    using ExpectedType = std::variant<float, bool>;
    static_assert(std::is_same_v<TestInput, ExpectedType>);
}

TEST(VariantInputBuilder, ValidatedVariantInput)
{
    using TestInput = ValidatedInputVariant<TestStructWithVariantTypes, IsInTestVariant>;
    static_assert(std::variant_size_v<TestInput> == 2);
    static_assert(std::is_same_v<std::variant_alternative_t<0, TestInput>, core::ValidatedInputTypeWrapper<float>>);
    static_assert(std::is_same_v<std::variant_alternative_t<1, TestInput>, core::ValidatedInputTypeWrapper<bool>>);
}
}  // namespace plato::core::unittest