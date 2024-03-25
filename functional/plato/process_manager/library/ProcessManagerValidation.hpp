#ifndef PLATO_PROCESSMANAGER_LIBRARY_PROCESSMANAGERVALIDATION
#define PLATO_PROCESSMANAGER_LIBRARY_PROCESSMANAGERVALIDATION

#include <optional>
#include <string>
#include <vector>

#include "plato/input_parser/InputBlocks.hpp"

namespace plato::process_manager::library
{
namespace detail
{
[[nodiscard]] std::optional<std::string> validate_at_least_one_process_manager(const input_parser::ParsedInput& aInput);
}

[[nodiscard]] std::vector<std::string> validate_process_managers(const input_parser::ParsedInput& aInput,
                                                                 std::vector<std::string>&& aCurrentMessageList);
}  // namespace plato::process_manager::library

#endif