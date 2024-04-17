#include <gtest/gtest.h>

#include <string_view>

#include "plato/core/FactoryRegistration.hpp"

namespace plato::core::unittest
{
namespace
{
constexpr auto kONegative = std::string_view{"type o negative"};
constexpr auto kBPositive = std::string_view{"type b negative"};

struct TestFactoryObject
{
    std::string mType;
    std::string mName;
};

using TestFactoryInput = std::string;
using TestFactoryRegistration = FactoryRegistration<TestFactoryObject, TestFactoryInput>;

[[maybe_unused]] static auto kFactoryTestRegistrationONegative =
    TestFactoryRegistration{std::string{kONegative}, [](const std::string& aName) {
                                return TestFactoryObject{std::string{kONegative}, aName};
                            }};

[[maybe_unused]] static auto kFactoryTestRegistrationBPositive =
    TestFactoryRegistration{std::string{kBPositive}, [](const std::string& aName) {
                                return TestFactoryObject{std::string{kBPositive}, aName};
                            }};

struct TestFactoryObject2
{
    std::string mType;
    std::string mName;
    int mNumber;
};
using TestFactoryInput2 = int;
using TestFactoryRegistrationMultipleArgs =
    FactoryRegistration<TestFactoryObject2, TestFactoryInput, TestFactoryInput2>;
[[maybe_unused]] static auto kFactoryTestRegistrationMultipleArgs =
    TestFactoryRegistrationMultipleArgs{std::string{kBPositive}, [](const std::string& aName, const int& aNumber) {
                                            return TestFactoryObject2{std::string{kBPositive}, aName, aNumber};
                                        }};

}  // namespace

TEST(FactoryRegistration, IsRegistered)
{
    EXPECT_TRUE((is_factory_function_registered<TestFactoryObject, TestFactoryInput>(kONegative)));
    EXPECT_TRUE((is_factory_function_registered<TestFactoryObject, TestFactoryInput>(kBPositive)));
    EXPECT_FALSE((is_factory_function_registered<TestFactoryObject, TestFactoryInput>("ab")));
    EXPECT_TRUE((is_factory_function_registered<TestFactoryObject2, TestFactoryInput, TestFactoryInput2>(kBPositive)));
}

TEST(FactoryRegistration, CreateObjectO)
{
    const auto tName = std::string{"jack"};
    const std::optional<TestFactoryObject> tTestObjectO =
        create_object_from_factory<TestFactoryObject, TestFactoryInput>(kONegative, tName);
    ASSERT_TRUE(tTestObjectO.has_value());
    EXPECT_EQ(tTestObjectO->mType, kONegative);
    EXPECT_EQ(tTestObjectO->mName, tName);
}

TEST(FactoryRegistration, CreateObjectB)
{
    const auto tName = std::string{"gizmo"};
    const std::optional<TestFactoryObject> tTestObjectB =
        create_object_from_factory<TestFactoryObject, TestFactoryInput>(kBPositive, tName);
    ASSERT_TRUE(tTestObjectB.has_value());
    EXPECT_EQ(tTestObjectB->mType, kBPositive);
    EXPECT_EQ(tTestObjectB->mName, tName);
}

TEST(FactoryRegistration, CreateObjectMultipleArgs)
{
    const auto tName = std::string{"king henry"};
    const auto tNumber = int{42};
    const std::optional<TestFactoryObject2> tTestObjectB =
        create_object_from_factory<TestFactoryObject2, TestFactoryInput, TestFactoryInput2>(kBPositive, tName, tNumber);
    ASSERT_TRUE(tTestObjectB.has_value());
    EXPECT_EQ(tTestObjectB->mType, kBPositive);
    EXPECT_EQ(tTestObjectB->mName, tName);
    EXPECT_EQ(tTestObjectB->mNumber, tNumber);
}

}  // namespace plato::core::unittest
