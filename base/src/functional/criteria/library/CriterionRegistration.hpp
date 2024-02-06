#ifndef PLATO_FUNCTIONAL_CRITERIONREGISTRATION
#define PLATO_FUNCTIONAL_CRITERIONREGISTRATION

#include <string_view>

#include "DynamicVector.hpp"
#include "FactoryRegistration.hpp"
#include "FileList.hpp"
#include "Function.hpp"

namespace plato::functional::core
{
struct MeshProxy;
}  // namespace plato::functional::core

namespace plato::functional::criteria::library
{
struct CriterionInput
{
    Plato::FileName mSharedLibraryPath;
    unsigned int mNumberOfProcessors;
    Plato::FileList mInputFiles;
};

using CriterionFunction = core::Function<double, linear_algebra::DynamicVector<double>, const core::MeshProxy&>;
using CriterionRegistration = core::FactoryRegistration<CriterionFunction, CriterionInput>;

bool is_criterion_function_registered(const std::string_view aFunctionName);

}  // namespace plato::functional::criteria::library

#endif
