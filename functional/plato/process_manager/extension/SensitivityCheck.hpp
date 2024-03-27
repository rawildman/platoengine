#ifndef PLATO_PROCESSMANAGER_EXTENSION_ROLSENSITIVITYCHECK
#define PLATO_PROCESSMANAGER_EXTENSION_ROLSENSITIVITYCHECK

#include <filesystem>
#include <optional>
#include <vector>

#include "plato/core/ValidatedInputTypeWrapper.hpp"

namespace plato::input_parser
{
struct sensitivity_check;
}

namespace plato::process_manager::library
{
struct ProcessManagerData;
}

namespace plato::process_manager::extension
{
class SensitivityCheck
{
   public:
    using ValidatedSensitivityCheckInput = core::ValidatedInputTypeWrapper<input_parser::sensitivity_check>;

    explicit SensitivityCheck(const ValidatedSensitivityCheckInput& aInput);

    void run(const library::ProcessManagerData& aProcessManagerData) const;

   private:
    std::filesystem::path mOutputFileName;
};

[[nodiscard]] std::vector<std::string> validate_sensitivity_check(const input_parser::sensitivity_check& aInput,
                                                                  std::vector<std::string>&& aCurrentMessageList);
namespace detail
{
[[nodiscard]] std::optional<std::string> validate_output_file_name(const input_parser::sensitivity_check& aInput);
}

}  // namespace plato::process_manager::extension

#endif
