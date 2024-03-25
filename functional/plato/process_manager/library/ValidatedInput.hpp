#ifndef PLATO_PROCESSMANAGER_LIBRARY_VALIDATEDINPUT
#define PLATO_PROCESSMANAGER_LIBRARY_VALIDATEDINPUT

#include <filesystem>

#include "plato/core/ValidatedInputTypeWrapper.hpp"
#include "plato/geometry/library/GeometryRegistration.hpp"
#include "plato/process_manager/library/ProcessManagerRegistration.hpp"
#include "plato/input_parser/InputBlocks.hpp"

namespace plato::process_manager::library
{
class ValidatedInput;

struct Key
{
    friend ValidatedInput make_validated_input(input_parser::ParsedInput input);

   private:
    Key() {}
    Key(const Key&) {}
};

class ValidatedInput
{
   public:
    using Geometry = plato::geometry::library::ValidatedGeometryInput;
    using Objectives =
        core::ValidatedInputTypeWrapper<std::vector<core::ValidatedInputTypeWrapper<input_parser::objective>>>;
    using Constraints =
        core::ValidatedInputTypeWrapper<std::vector<core::ValidatedInputTypeWrapper<input_parser::constraint>>>;
    using OptimizationParameters = core::ValidatedInputTypeWrapper<input_parser::optimization_parameters>;
    using ProcessManagers = plato::process_manager::library::ValidatedProcessManagerInput;

   public:
    ValidatedInput(input_parser::ParsedInput aInput, Key);

    [[nodiscard]] auto geometry() const -> Geometry;
    [[nodiscard]] auto objectives() const -> Objectives;
    [[nodiscard]] auto constraints() const -> Constraints;
    [[nodiscard]] auto optimizationParameters() const -> OptimizationParameters;
    [[nodiscard]] auto processManagers() const -> ProcessManagers;

   private:
    template <typename T>
    [[nodiscard]] static std::vector<core::ValidatedInputTypeWrapper<T>> validatedVector(const std::vector<T>& aInputs);

    template <typename ValidatedInputVariant, typename InputVariant>
    [[nodiscard]] static ValidatedInputVariant validatedVariant(InputVariant aInputVariant);

   private:
    input_parser::ParsedInput mInput;
};

[[nodiscard]] ValidatedInput make_validated_input(input_parser::ParsedInput aInput);

/// @brief Parse input from file @a aInputFile and then validate the input
[[nodiscard]] ValidatedInput parse_and_validate_from_file(const std::filesystem::path& aFileName);

/// @brief Parse input from input string @a aInput and then validate the input
[[nodiscard]] ValidatedInput parse_and_validate(const std::string_view aInput);

}  // namespace plato::process_manager::library

#endif
