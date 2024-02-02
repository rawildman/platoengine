#ifndef PLATO_FUNCTIONAL_CRITERIONREGISTRATION
#define PLATO_FUNCTIONAL_CRITERIONREGISTRATION

#include <string_view>

#include "DynamicVector.hpp"
#include "FactoryRegistration.hpp"
#include "Function.hpp"
#include "FileList.hpp"

namespace Plato::Functional
{
struct MeshProxy;
}  // namespace Plato::Functional

namespace Plato::Functional::CriterionFactory
{
struct CriterionInput
{
    Plato::FileName mSharedLibraryPath;
    unsigned int mNumberOfProcessors;
    Plato::FileList mInputFiles;
};

using CriterionFunction = Function<double, Core::DynamicVector<double>, const MeshProxy&>;
using CriterionRegistration = Registration<CriterionFunction, CriterionInput>;

bool is_criterion_function_registered(const std::string_view aFunctionName);

}  // namespace Plato::Functional::CriterionFactory

#endif
