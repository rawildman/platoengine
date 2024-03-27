#ifndef PLATO_PROCESSMANAGER_EXTENSION_ROLOPTIMIZATION
#define PLATO_PROCESSMANAGER_EXTENSION_ROLOPTIMIZATION

#include <Teuchos_ParameterList.hpp>

#include "plato/core/ValidatedInputTypeWrapper.hpp"

namespace plato::input_parser
{
struct optimization_parameters;
}

namespace plato::process_manager::library
{
struct ProcessManagerData;
}

namespace plato::process_manager::extension
{
class ROLOptimization
{
   public:
    using ValidatedOptimizationParameters = core::ValidatedInputTypeWrapper<input_parser::optimization_parameters>;

    explicit ROLOptimization(const ValidatedOptimizationParameters& aInput);

    void run(const library::ProcessManagerData& aProcessManagerData) const;

   private:
    Teuchos::ParameterList mROLOptions;
};

}  // namespace plato::process_manager::extension

#endif