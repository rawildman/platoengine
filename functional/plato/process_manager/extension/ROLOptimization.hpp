#ifndef PLATO_PROCESSMANAGER_EXTENSION_ROLOPTIMIZATION
#define PLATO_PROCESSMANAGER_EXTENSION_ROLOPTIMIZATION

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
    explicit ROLOptimization(const input_parser::optimization_parameters& aInput);

    void run(const library::ProcessManagerData& aProcessManagerData) const;
};

}  // namespace plato::process_manager::extension

#endif