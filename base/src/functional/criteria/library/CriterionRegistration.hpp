#ifndef PLATO_FUNCTIONAL_CRITERIA_LIBRARY_CRITERIONREGISTRATION
#define PLATO_FUNCTIONAL_CRITERIA_LIBRARY_CRITERIONREGISTRATION

#include <string_view>

#include "linear_algebra/DynamicVector.hpp"
#include "core/FactoryRegistration.hpp"
#include "input_parser/FileList.hpp"
#include "core/Function.hpp"

namespace plato::functional::core
{
struct MeshProxy;
}  // namespace plato::functional::core

namespace plato::functional::criteria::library
{
struct CriterionInput
{
    input_parser::FileName mSharedLibraryPath;
    unsigned int mNumberOfProcessors;
    input_parser::FileList mInputFiles;
};

using CriterionFunction = core::Function<double, linear_algebra::DynamicVector<double>, const core::MeshProxy&>;
using CriterionRegistration = core::FactoryRegistration<CriterionFunction, CriterionInput>;

bool is_criterion_function_registered(const std::string_view aFunctionName);

}  // namespace plato::functional::criteria::library

#endif
