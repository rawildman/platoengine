#include <gtest/gtest.h>

#include <variant>

#include "plato/input_parser/InputBlocks.hpp"
#include "plato/process_manager/library/ProcessManagerRegistration.hpp"

namespace plato::process_manager::library::unittest
{
namespace
{

[[nodiscard]] auto make_test_process_manager_function(const library::ValidatedProcessManagerInput&) -> ProcessManager
{
    return [](const ProcessManagerData&) {};
}

[[maybe_unused]] static auto kTestProcessManagerRegistration =
    ProcessManagerRegistration{"test", [](const ValidatedProcessManagerInput& aValidInput)
                               { return make_test_process_manager_function(aValidInput); }};
}  // namespace

TEST(ProcessManagerRegistration, TestProcessManager) { EXPECT_TRUE(is_process_manager_function_registered("test")); }

TEST(ProcessManagerRegistration, NonexistantProcessManager)
{
    EXPECT_FALSE(is_process_manager_function_registered("flem"));
}

TEST(ProcessManagerRegistration, ProcessManagerOptimizationByDefaultNotEmpty)
{
    EXPECT_EQ(process_manager_input(input_parser::ParsedInput{}).size(), 1u);
}

TEST(ProcessManagerRegistration, ProcessManagerInput)
{
    using TestInput = ProcessManagerInput;
    static_assert(std::variant_size_v<TestInput> == 2);
    static_assert(std::is_same_v<std::variant_alternative_t<0, TestInput>, input_parser::gradient_check>);
    static_assert(std::is_same_v<std::variant_alternative_t<1, TestInput>, input_parser::optimization_parameters>);
}

TEST(ProcessManagerRegistration, ValidatedProcessManagerInputVector)
{
    using TestInput = typename ValidatedProcessManagerInputVector::RawInputType::value_type;
    static_assert(std::variant_size_v<TestInput> == 2);
    static_assert(std::is_same_v<std::variant_alternative_t<0, TestInput>,
                                 core::ValidatedInputTypeWrapper<input_parser::gradient_check>>);
    static_assert(std::is_same_v<std::variant_alternative_t<1, TestInput>,
                                 core::ValidatedInputTypeWrapper<input_parser::optimization_parameters>>);
}
}  // namespace plato::process_manager::library::unittest