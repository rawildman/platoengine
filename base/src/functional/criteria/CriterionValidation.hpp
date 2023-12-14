#ifndef PLATO_FUNCTIONAL_CRITERIAVALIDATION
#define PLATO_FUNCTIONAL_CRITERIAVALIDATION

#include "Plato_InputBlocks.hpp"
#include "ValidationRegistration.hpp"

namespace Plato::Functional::Criteria
{

namespace detail
{

namespace
{
#define RETRIEVE_OBJECT_NAME(aObject) demangled(typeid(aObject).name())
std::string demangled(char const* aName)
{
    std::unique_ptr<char, void (*)(void*)> tName{abi::__cxa_demangle(aName, 0, 0, nullptr), std::free};
    return {tName.get()};
}
}  // namespace

template <typename Criteria>
std::string prepend_string(const Criteria& aInput)
{
    std::string tName = aInput.name.value_or("unnamed");
    return RETRIEVE_OBJECT_NAME(aInput) + " " + tName;
}

template <typename Criteria>
std::optional<std::string> validate_app(const Criteria& aInput)
{
    return Plato::Functional::Affirmations::error_message_for_empty_parameter(prepend_string<Criteria>(aInput),
                                                                              aInput.app, "app");
}

template <typename Criteria>
std::optional<std::string> validate_custom_app(const Criteria& aInput)
{
    if (aInput.app.has_value() && aInput.app.value() == Plato::CodeOptions::kCustomApp)
    {
        return Plato::Functional::Affirmations::error_message_for_empty_parameter(
            prepend_string<Criteria>(aInput), aInput.shared_library_path, "shared_library_path");
    }
    else
    {
        return std::nullopt;
    }
}

template <typename Criteria>
std::vector<std::string> validate_criteria(const std::vector<Criteria>& aInput,
                                           const std::vector<std::string>& aCurrentMessageList)
{
    std::vector<std::string> tMessageList = aCurrentMessageList;
    tMessageList = Plato::Functional::Validation::validate<std::vector<Criteria>>(aInput, tMessageList);

    for (const auto& iCriterionInput : aInput)
    {
        tMessageList = Plato::Functional::Validation::validate<Criteria>(iCriterionInput, tMessageList);
    }
    return tMessageList;
}

}  // namespace detail

}  // namespace Plato::Functional::Criteria

#endif