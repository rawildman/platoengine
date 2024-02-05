#include "ObjectiveValidation.hpp"

#include <boost/mpi/communicator.hpp>

#include "CriterionValidation.hpp"

namespace Plato::Functional::Criteria
{
[[maybe_unused]] static auto kObjectiveValidationRegistration =
    Plato::Functional::Validation::Registration<Plato::objective>{
        [](const Plato::objective& aInput) { return detail::validate_app(aInput); },
        [](const Plato::objective& aInput) { return detail::validate_custom_app(aInput); },
        [](const Plato::objective& aInput) { return detail::validate_number_of_processors(aInput); },
        [](const Plato::objective& aInput) { return detail::validate_aggregation_weight(aInput); }};

[[maybe_unused]] static auto kListObjectivesValidationRegistration =
    Plato::Functional::Validation::Registration<std::vector<Plato::objective>>{
        [](const std::vector<Plato::objective>& aInput) { return detail::validate_at_least_one_objective(aInput); },
        [](const std::vector<Plato::objective>& aInput) { return detail::validate_number_of_ranks_vs_objectives(aInput); }};

std::vector<std::string> validate_objectives(const std::vector<Plato::objective>& aInput,
                                             std::vector<std::string>&& aCurrentMessageList)
{
    return detail::validate_criteria(aInput, std::move(aCurrentMessageList));
}

namespace detail
{
std::optional<std::string> validate_aggregation_weight(const Plato::objective& aInput)
{
    namespace pfv = Plato::Functional::Validation;
    namespace pfc = Plato::Functional::Core;
    return pfv::error_message_for_parameter_out_of_bounds(criterion_name(aInput), aInput.aggregation_weight,
                                                          "aggregation_weight",
                                                          pfc::lower_bounded(pfc::Exclusive{0.0}));
}

std::optional<std::string> validate_at_least_one_objective(const std::vector<Plato::objective>& aInput)
{
    const bool tAnyActiveObjectives = std::any_of(aInput.begin(), aInput.end(),
                                                  [](const Plato::objective& aObjective)
                                                  { return Plato::Functional::Validation::is_active(aObjective); });

    if (tAnyActiveObjectives)
    {
        return std::nullopt;
    }
    else
    {
        return "No active objectives found.";
    }
}

std::optional<std::string> validate_number_of_ranks_vs_objectives(const std::vector<Plato::objective>& aInput)
{
    const auto tNumRanks = boost::mpi::communicator{}.size();
    if (tNumRanks < 0 || static_cast<std::size_t>(tNumRanks) > aInput.size())
    {
        return "The number of MPI ranks exceeds the number of objectives.";
    }
    else
    {
        return std::nullopt;
    }
}

}  // namespace detail
}  // namespace Plato::Functional::Criteria
