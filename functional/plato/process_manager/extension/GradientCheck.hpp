#ifndef PLATO_PROCESSMANAGER_EXTENSION_GRADIENTCHECK
#define PLATO_PROCESSMANAGER_EXTENSION_GRADIENTCHECK

#include <filesystem>
#include <optional>

#include "plato/core/ValidationUtilities.hpp"
#include "plato/input_parser/InputBlocks.hpp"

namespace plato::input_parser
{
struct gradient_check;
}

namespace plato::process_manager::library
{
struct ProcessManagerData;
}

namespace plato::process_manager::extension
{

class GradientCheck
{
   public:
    explicit GradientCheck(const input_parser::gradient_check& aInput);

    void run(const library::ProcessManagerData& aProcessManagerData) const;

   private:
    std::filesystem::path mOutputFileName;
    unsigned int mNumberOfSteps = 12;
    double mInitialDirectionMagnitude = 1;
    double mStepSizeReductionFactor = 0.1;
    unsigned int mRandomDirectionSeed = 42;
};

[[nodiscard]] std::vector<std::string> validate_gradient_check(const input_parser::gradient_check& aInput,
                                                               std::vector<std::string>&& aCurrentMessageList);
namespace detail
{

[[nodiscard]] std::optional<std::string> validate_output_file_name(const input_parser::gradient_check& aInput);
[[nodiscard]] std::optional<std::string> validate_number_of_steps(const input_parser::gradient_check& aInput);
[[nodiscard]] std::optional<std::string> validate_initial_direction_magnitude(
    const input_parser::gradient_check& aInput);
[[nodiscard]] std::optional<std::string> validate_step_size_reduction_factor(
    const input_parser::gradient_check& aInput);
[[nodiscard]] std::optional<std::string> validate_random_direction_seed(const input_parser::gradient_check& aInput);

}  // namespace detail

}  // namespace plato::process_manager::extension
#endif