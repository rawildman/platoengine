#ifndef PLATO_FUNCTIONAL_CRITERIONREGISTRATION
#define PLATO_FUNCTIONAL_CRITERIONREGISTRATION

#include <ROL_StdVector.hpp>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include "FactoryRegistration.hpp"
#include "Function.hpp"
#include "Plato_FileList.hpp"

namespace Plato
{
struct objective;
struct constraint;
namespace Functional
{
struct MeshProxy;
}
}  // namespace Plato

namespace Plato::Functional::CriterionFactory
{
struct CriterionInput
{
    Plato::FileName mSharedLibraryPath;
    unsigned int mNumberOfProcessors;
    Plato::FileList mInputFiles;
};

using CriterionFunction = Function<double, ROL::StdVector<double>, const MeshProxy&>;
using CriterionRegistration = Registration<CriterionFunction, CriterionInput>;


bool is_criterion_function_registered(const std::string_view aFunctionName);


}  // namespace Plato::Functional::CriterionFactory

#endif
