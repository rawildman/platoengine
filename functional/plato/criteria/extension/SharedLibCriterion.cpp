#include "plato/criteria/extension/SharedLibCriterion.hpp"

#include <boost/mpi/communicator.hpp>

#include "plato/utilities/SharedLibraryUtilities.hpp"

namespace plato::criteria::extension
{
namespace
{
template <typename FunctionPtr, typename... Args>
std::unique_ptr<library::CriterionInterface> load_criterion_interface(
    const std::filesystem::path& aSharedLibPath, const std::string_view aCreateCriterionFunctionName, Args&&... aArgs)
{
    void* const tSharedLibInterface = plato::utilities::load_shared_library(aSharedLibPath);
    const auto tCreateCriterionFunction =
        plato::utilities::load_function<FunctionPtr>(tSharedLibInterface, aCreateCriterionFunctionName, aSharedLibPath);
    return tCreateCriterionFunction(std::forward<Args>(aArgs)...);
}

using CreateSerialCriterionFunction =
    std::add_pointer_t<std::unique_ptr<library::CriterionInterface>(const std::vector<std::string>&)>;

using CreateParallelCriterionFunction =
    std::add_pointer_t<std::unique_ptr<library::CriterionInterface>(const std::vector<std::string>&, MPI_Comm)>;

}  // namespace

SharedLibCriterion::SharedLibCriterion(const std::filesystem::path& aSharedLibPath,
                                       const std::vector<std::string>& aFileNames)
    : mCriterionInterface{load_criterion_interface<CreateSerialCriterionFunction>(
          aSharedLibPath, library::kCreateCriterionFunctionName, aFileNames)}
{
}

SharedLibCriterion::SharedLibCriterion(const std::filesystem::path& aSharedLibPath,
                                       const std::vector<std::string>& aFileNames,
                                       const boost::mpi::communicator& aComm)
    : mCriterionInterface{load_criterion_interface<CreateParallelCriterionFunction>(
          aSharedLibPath, library::kCreateParallelCriterionFunctionName, aFileNames, aComm)},
      mComm{aComm}
{
}

double SharedLibCriterion::f(const core::MeshProxy& aMesh) const { return mCriterionInterface->value(aMesh); }

linear_algebra::DynamicVector<double> SharedLibCriterion::df(const core::MeshProxy& aMesh) const
{
    std::vector<double> tGradient = mCriterionInterface->gradient(aMesh);
    return linear_algebra::DynamicVector<double>(std::move(tGradient));
}

auto make_shared_lib_function(const SharedLibCriterion& aSharedLibCriterion)
    -> core::Function<double, linear_algebra::DynamicVector<double>, const core::MeshProxy&>
{
    return core::make_function(
        [aSharedLibCriterion](const core::MeshProxy& mesh) { return aSharedLibCriterion.f(mesh); },
        [aSharedLibCriterion](const core::MeshProxy& mesh) { return aSharedLibCriterion.df(mesh); });
}

}  // namespace plato::criteria::extension
