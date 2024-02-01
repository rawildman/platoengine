#ifndef PLATO_FUNCTIONAL_CRITERIONFACTORY
#define PLATO_FUNCTIONAL_CRITERIONFACTORY

#include <string>

#include "CriterionRegistration.hpp"
#include "DynamicVector.hpp"
#include "Exception.hpp"
#include "Function.hpp"
#include "Plato_InputBlocks.hpp"
#include "ValidatedInputTypeWrapper.hpp"

namespace Plato::Functional::CriterionFactory
{
using CriterionFunction = Function<double, Core::DynamicVector<double>, const MeshProxy&>;

/// @brief Converts either objective or constraint input objects to a common CriterionInput struct
/// @tparam Input Must be either Plato::objective or Plato::constraint input structs
template <typename Input>
[[nodiscard]] CriterionInput to_criterion_input(const Input& aInput);

/// @brief Creates a criterion Function object from either objective or constraint input objects.
/// @tparam Input Must be either Plato::objective or Plato::constraint input structs
template <typename Input>
[[nodiscard]] CriterionFunction make_criterion_function(const Input& aInput);

template <typename Input>
CriterionFunction make_criterion_function(const Input& aValidatedInput)
{
    static_assert(std::is_same_v<Input, Core::ValidatedInputTypeWrapper<Plato::objective>> ||
                      std::is_same_v<Input, Core::ValidatedInputTypeWrapper<Plato::constraint>>,
                  "make_criterion_function must only be called with Plato::objective or Plato::constraint wrapped in "
                  "ValidatedInputTypeWrapper");

    const auto& tRawInput = aValidatedInput.rawInput();
    const std::string tAppName = Plato::kCodeOptionsTable.toString(tRawInput.app.value()).value();
    if (const auto tIter = detail::registered_functions<CriterionFunction, CriterionInput>().find(tAppName);
        tIter != detail::registered_functions<CriterionFunction, CriterionInput>().end())
    {
        return tIter->second(to_criterion_input(aValidatedInput));
    }
    else
    {
        throw Plato::Functional::Exception("App not supported.");
    }
}

template <typename Input>
CriterionInput to_criterion_input(const Input& aInput)
{
    static_assert(std::is_same_v<Input, Core::ValidatedInputTypeWrapper<Plato::objective>> ||
                      std::is_same_v<Input, Core::ValidatedInputTypeWrapper<Plato::constraint>>,
                  "to_criterion_input must only be called with Plato::objective or Plato::constraint wrapped in "
                  "ValidatedInputTypeWrapper");
    return CriterionInput{/*.mSharedLibraryPath=*/aInput.rawInput().shared_library_path.value_or(Plato::FileName{}),
                          /*.mNumberOfProcessors=*/aInput.rawInput().number_of_processors.value_or(1),
                          /*.mInputFiles=*/aInput.rawInput().input_files.value_or(Plato::FileList{})};
}

}  // namespace Plato::Functional::CriterionFactory

#endif
