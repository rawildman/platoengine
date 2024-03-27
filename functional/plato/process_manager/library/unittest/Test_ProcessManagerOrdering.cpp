#include <gtest/gtest.h>

namespace plato::process_manager::library::unittest
{
enum class RunStage
{
    kValidate,
    kExecute,
    kPostProcess
};

TEST(ProcessManagerOrdering, SingleStage)
{
//    std::multimap<RunStage, ProcessManager> 
}
}
