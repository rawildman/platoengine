#ifndef PLATO_PROCESSMANAGER_LIBRARY_STAGEORDERING
#define PLATO_PROCESSMANAGER_LIBRARY_STAGEORDERING

#include <algorithm>
#include <iterator>
#include <map>
#include <vector>

namespace plato::process_manager::library
{
enum class RunStage
{
    kValidate,
    kExecute,
    kPostProcess
};

/// @brief Converts @a aStageMap to a vector so that each item in the map is in order in the vector
/// according to its RunStage. The order within stages is not guaranteed, only that all, e.g.,
/// RunStage::kValidate items are before all RunStage::kExecute items.
template <typename T>
[[nodiscard]] std::vector<T> to_stage_ordered_vector(std::multimap<RunStage, T> aStageMap);

template <typename T>
std::vector<T> to_stage_ordered_vector(std::multimap<RunStage, T> aStageMap)
{
    std::vector<T> tResult;
    tResult.reserve(aStageMap.size());
    std::transform(aStageMap.begin(), aStageMap.end(), std::back_inserter(tResult),
                   [](auto& tStageAndItem) { return std::move(tStageAndItem.second); });
    return tResult;
}
}  // namespace plato::process_manager::library

#endif
