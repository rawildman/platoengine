#ifndef PLATO_MAIN_LIBRARY_EXECUTOR
#define PLATO_MAIN_LIBRARY_EXECUTOR

#include <functional>
#include <initializer_list>
#include <vector>

namespace plato::process_manager::library
{
struct ProcessManagerData;
}

namespace plato::main::library
{
template <typename Data>
class Executor
{
   public:
    using FunctionType = std::function<void(Data&)>;

    explicit Executor(std::vector<FunctionType> tProcessManagers);

    void execute(Data& aProcessManagerData) const;

   private:
    std::vector<FunctionType> mProcessManagers;
};

template <typename Data>
Executor<Data>::Executor(std::vector<FunctionType> tProcessManagers)
    : mProcessManagers{std::move(tProcessManagers)}
{
}

template <typename Data>
void Executor<Data>::execute(Data& aData) const
{
    for (const auto& tProcessManager : mProcessManagers)
    {
        tProcessManager(aData);
    }
}

}  // namespace plato::main::library

#endif
