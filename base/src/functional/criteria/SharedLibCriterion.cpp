#include "SharedLibCriterion.hpp"

#include "CriterionRegistration.hpp"
#include "Plato_InputEnumTypes.hpp"
#include "Exception.hpp"
#include "SharedLibraryUtilities.hpp"

namespace Plato::Functional
{
namespace
{
SharedLibCriterion make_shared_lib_criterion(const Plato::Functional::CriterionFactory::CriterionInput& aInput)
{
    return SharedLibCriterion{aInput.mSharedLibraryPath.mName, aInput.mInputFiles.mList};
}

[[maybe_unused]] static auto kCustomAppRegistration =
    CriterionFactory::CriterionRegistration{Plato::kCodeOptionsTable.toString(Plato::CodeOptions::kCustomApp).value(),
                                            [](const Plato::Functional::CriterionFactory::CriterionInput& aInput)
                                            { return make_shared_lib_function(make_shared_lib_criterion(aInput)); }};
}  // namespace

SharedLibCriterion::SharedLibCriterion(const std::filesystem::path& aSharedLibPath,
                                       const std::vector<std::string>& aFileNames)
    : mSharedLibPath(aSharedLibPath)
{
    using CreateCriterionFunction =
        std::add_pointer_t<std::unique_ptr<CriterionInterface>(const std::vector<std::string>&)>;

    void* const tSharedLibInterface = Utilities::load_shared_library(aSharedLibPath);
    const auto tCreateCriterionFunction = Utilities::load_function<CreateCriterionFunction>(
        tSharedLibInterface, kCreateCriterionFunctionName, aSharedLibPath);
    mCriterionFunction = tCreateCriterionFunction(aFileNames);
}

double SharedLibCriterion::f(const MeshProxy& aMesh) const { return mCriterionFunction->value(aMesh); }

ROL::StdVector<double> SharedLibCriterion::df(const MeshProxy& aMesh) const
{
    std::vector<double> tGradient = mCriterionFunction->gradient(aMesh);
    return ROL::StdVector<double>(ROL::makePtr<std::vector<double>>(std::move(tGradient)));
}

auto make_shared_lib_function(const SharedLibCriterion& aSharedLibCriterion)
    -> Function<double, ROL::StdVector<double>, const MeshProxy&>
{
    return make_function([aSharedLibCriterion](const MeshProxy& mesh) { return aSharedLibCriterion.f(mesh); },
                         [aSharedLibCriterion](const MeshProxy& mesh) { return aSharedLibCriterion.df(mesh); });
}

}  // namespace Plato::Functional
