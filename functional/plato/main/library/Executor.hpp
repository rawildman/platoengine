#ifndef PLATO_MAIN_LIBRARY_EXECUTOR
#define PLATO_MAIN_LIBRARY_EXECUTOR

#include <functional>
#include <vector>

namespace plato::process_manager::library
{
struct ProcessManagerData;
}

namespace plato::main::library
{
/// @brief Executes a series of functions defined on construction.
///
/// Each function has the signature `void(Data&)`, where Data is the template parameter
/// of this class. The main purpose of this class is to execute a set of ProcessManager
/// objects, which are defined as functions with the signature `void(const ProcessManager&)`.
template <typename Data>
class Executor
{
   public:
    using FunctionType = std::function<void(Data&)>;

    /// @brief Construct with a set of functions, the order of @a aFunctions is the order
    ///  in which they will be executed when execute is called.
    explicit Executor(std::vector<FunctionType> aFunctions);

    /// @brief Execute each function defined on construction in order, with the argument
    ///  @a aData passed to each function.
    void execute(Data& aData) const;

   private:
    std::vector<FunctionType> mFunctions;
};

template <typename Data>
Executor<Data>::Executor(std::vector<FunctionType> aFunctions) : mFunctions{std::move(aFunctions)}
{
}

template <typename Data>
void Executor<Data>::execute(Data& aData) const
{
    for (const auto& tFunction : mFunctions)
    {
        tFunction(aData);
    }
}

}  // namespace plato::main::library

#endif
