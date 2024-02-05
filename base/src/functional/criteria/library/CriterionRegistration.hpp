#ifndef PLATO_FUNCTIONAL_CRITERIONREGISTRATION
#define PLATO_FUNCTIONAL_CRITERIONREGISTRATION

#include <string_view>

#include "DynamicVector.hpp"
#include "FactoryRegistration.hpp"
#include "FileList.hpp"
#include "Function.hpp"

namespace Plato::Functional
{
struct MeshProxy;
}  // namespace Plato::Functional

namespace plato::functional::criteria::library
{
struct CriterionInput
{
    Plato::FileName mSharedLibraryPath;
    unsigned int mNumberOfProcessors;
    Plato::FileList mInputFiles;
};

using CriterionFunction = Plato::Functional::
    Function<double, linear_algebra::DynamicVector<double>, const Plato::Functional::MeshProxy&>;
using CriterionRegistration = Plato::Functional::Registration<CriterionFunction, CriterionInput>;

bool is_criterion_function_registered(const std::string_view aFunctionName);

}  // namespace plato::functional::criteria::library

#endif
