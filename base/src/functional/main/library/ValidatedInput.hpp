#ifndef PLATO_FUNCTIONAL_MAIN_LIBRARY_VALIDATEDINPUT
#define PLATO_FUNCTIONAL_MAIN_LIBRARY_VALIDATEDINPUT

#include <filesystem>

#include "GeometryRegistration.hpp"
#include "InputBlocks.hpp"
#include "ValidatedInputTypeWrapper.hpp"

namespace plato::functional::main::library
{
class ValidatedInput;

struct Key
{
    friend ValidatedInput make_validated_input(Plato::PlatoInput input);

   private:
    Key() {}
    Key(const Key&) {}
};

class ValidatedInput
{
   public:
    using Geometry = plato::functional::geometry::library::ValidatedGeometryInput;
    using Objectives = core::ValidatedInputTypeWrapper<
        std::vector<core::ValidatedInputTypeWrapper<Plato::objective>>>;
    using Constraints = core::ValidatedInputTypeWrapper<
        std::vector<core::ValidatedInputTypeWrapper<Plato::constraint>>>;
    using OptimizationParameters = core::ValidatedInputTypeWrapper<Plato::optimization_parameters>;

   public:
    ValidatedInput(Plato::PlatoInput aInput, Key);

    [[nodiscard]] auto geometry() const -> Geometry;
    [[nodiscard]] auto objectives() const -> Objectives;
    [[nodiscard]] auto constraints() const -> Constraints;
    [[nodiscard]] auto optimizationParameters() const -> OptimizationParameters;

   private:
    template <typename T>
    static std::vector<core::ValidatedInputTypeWrapper<T>> validatedVector(
        const std::vector<T>& aInputs);

   private:
    Plato::PlatoInput mInput;
};

[[nodiscard]] ValidatedInput make_validated_input(Plato::PlatoInput aInput);

/// @brief Parse input from file @a aInputFile and then validate the input
[[nodiscard]] ValidatedInput parse_and_validate_from_file(const std::filesystem::path& aFileName);

/// @brief Parse input from input string @a aInput and then validate the input
[[nodiscard]] ValidatedInput parse_and_validate(const std::string_view aInput);

}  // namespace plato::functional::main::library

#endif
