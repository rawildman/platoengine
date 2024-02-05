#include "SharedLibCriterion.hpp"

#include "CriterionRegistration.hpp"
#include "Exception.hpp"
#include "InputEnumTypes.hpp"
#include "SharedLibraryUtilities.hpp"

namespace plato::functional::criteria::extension
{
namespace
{
SharedLibCriterion make_shared_lib_criterion(const plato::functional::criteria::library::CriterionInput& aInput)
{
    return SharedLibCriterion{aInput.mSharedLibraryPath.mName, aInput.mInputFiles.mList};
}

[[maybe_unused]] static auto kCustomAppRegistration =
    library::CriterionRegistration{Plato::kCodeOptionsTable.toString(Plato::CodeOptions::kCustomApp).value(),
                                   [](const plato::functional::criteria::library::CriterionInput& aInput)
                                   { return make_shared_lib_function(make_shared_lib_criterion(aInput)); }};
}  // namespace

SharedLibCriterion::SharedLibCriterion(const std::filesystem::path& aSharedLibPath,
                                       const std::vector<std::string>& aFileNames)
    : mSharedLibPath(aSharedLibPath)
{
    using CreateCriterionFunction =
        std::add_pointer_t<std::unique_ptr<library::CriterionInterface>(const std::vector<std::string>&)>;

    void* const tSharedLibInterface = Plato::Functional::Utilities::load_shared_library(aSharedLibPath);
    const auto tCreateCriterionFunction = Plato::Functional::Utilities::load_function<CreateCriterionFunction>(
        tSharedLibInterface, library::kCreateCriterionFunctionName, aSharedLibPath);
    mCriterionFunction = tCreateCriterionFunction(aFileNames);
}

double SharedLibCriterion::f(const Plato::Functional::MeshProxy& aMesh) const
{
    return mCriterionFunction->value(aMesh);
}

Plato::Functional::Core::DynamicVector<double> SharedLibCriterion::df(const Plato::Functional::MeshProxy& aMesh) const
{
    std::vector<double> tGradient = mCriterionFunction->gradient(aMesh);
    return Plato::Functional::Core::DynamicVector<double>(std::move(tGradient));
}

auto make_shared_lib_function(const SharedLibCriterion& aSharedLibCriterion) -> Plato::Functional::
    Function<double, Plato::Functional::Core::DynamicVector<double>, const Plato::Functional::MeshProxy&>
{
    return Plato::Functional::make_function(
        [aSharedLibCriterion](const Plato::Functional::MeshProxy& mesh) { return aSharedLibCriterion.f(mesh); },
        [aSharedLibCriterion](const Plato::Functional::MeshProxy& mesh) { return aSharedLibCriterion.df(mesh); });
}

}  // namespace plato::functional::criteria::extension
