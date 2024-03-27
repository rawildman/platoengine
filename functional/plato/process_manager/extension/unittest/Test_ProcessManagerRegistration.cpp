#include <gtest/gtest.h>

#include <variant>

#include "plato/input_parser/InputBlocks.hpp"
#include "plato/process_manager/library/ProcessManagerRegistration.hpp"

namespace plato::process_manager::library::unittest
{

TEST(ProcessManagerRegistration, ProcessManagerInput)
{
    using TestInput = ProcessManagerInput;
    static_assert(std::variant_size_v<TestInput> == 3);
    static_assert(std::is_same_v<std::variant_alternative_t<1, TestInput>, input_parser::gradient_check>);
    static_assert(std::is_same_v<std::variant_alternative_t<2, TestInput>, input_parser::optimization_parameters>);
    static_assert(std::is_same_v<std::variant_alternative_t<0, TestInput>, input_parser::sensitivity_check>);
}

TEST(ProcessManagerRegistration, ValidatedProcessManagerInputVector)
{
    using TestInput = typename ValidatedProcessManagerInputVector::RawInputType::value_type;
    static_assert(std::variant_size_v<TestInput> == 3);
    static_assert(std::is_same_v<std::variant_alternative_t<1, TestInput>,
                                 core::ValidatedInputTypeWrapper<input_parser::gradient_check>>);
    static_assert(std::is_same_v<std::variant_alternative_t<2, TestInput>,
                                 core::ValidatedInputTypeWrapper<input_parser::optimization_parameters>>);
    static_assert(std::is_same_v<std::variant_alternative_t<0, TestInput>,
                                 core::ValidatedInputTypeWrapper<input_parser::sensitivity_check>>);
}
}  // namespace plato::process_manager::library::unittest