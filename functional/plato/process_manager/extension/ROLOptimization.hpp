#ifndef PLATO_PROCESSMANAGER_EXTENSION_ROLOPTIMIZATION
#define PLATO_PROCESSMANAGER_EXTENSION_ROLOPTIMIZATION

#include <Teuchos_ParameterList.hpp>
#include <boost/mpi/communicator.hpp>

#include "plato/core/ValidatedInputTypeWrapper.hpp"

namespace plato::input_parser
{
struct rol_optimization;
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
    using ValidatedOptimizationParameters = core::ValidatedInputTypeWrapper<input_parser::rol_optimization>;

    explicit ROLOptimization(const ValidatedOptimizationParameters& aInput);

    void run(const library::ProcessManagerData& aProcessManagerData) const;

   private:
    Teuchos::ParameterList mROLOptions;
    boost::mpi::communicator mCommunicator{};
};

}  // namespace plato::process_manager::extension

#endif