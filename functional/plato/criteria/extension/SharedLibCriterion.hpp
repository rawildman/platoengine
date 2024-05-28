#ifndef PLATO_CRITERIA_EXTENSION_SHAREDLIBCRITERION
#define PLATO_CRITERIA_EXTENSION_SHAREDLIBCRITERION

#include <boost/mpi/communicator.hpp>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "plato/core/Function.hpp"
#include "plato/core/MeshProxy.hpp"
#include "plato/core/ValidationRegistration.hpp"
#include "plato/core/ValidationUtilities.hpp"
#include "plato/criteria/library/CriterionInterface.hpp"
#include "plato/criteria/library/CriterionValidation.hpp"
#include "plato/linear_algebra/DynamicVector.hpp"

namespace plato::criteria::extension
{
/// @brief for a criterion that is loaded from a shared library.
///
/// A shared library path is given on construction from which to load
/// a CriterionInterface object.
class SharedLibCriterion
{
   public:
    SharedLibCriterion(const std::filesystem::path& aSharedLibPath, const std::vector<std::string>& aFileNames);
    SharedLibCriterion(const std::filesystem::path& aSharedLibPath,
                       const std::vector<std::string>& aFileNames,
                       const boost::mpi::communicator& aComm);

    [[nodiscard]] double f(const core::MeshProxy& aMesh) const;

    [[nodiscard]] linear_algebra::DynamicVector<double> df(const core::MeshProxy& aMesh) const;

    static constexpr auto kAppName = std::string_view{"custom_app"};

   private:
    std::shared_ptr<library::CriterionInterface> mCriterionInterface;
    boost::mpi::communicator mComm{MPI_COMM_NULL, boost::mpi::comm_attach};
};

[[nodiscard]] auto make_shared_lib_function(const SharedLibCriterion& aSharedLibCriterion)
    -> core::Function<double, linear_algebra::DynamicVector<double>, const core::MeshProxy&>;

namespace detail
{
template <typename Criteria>
[[nodiscard]] std::optional<std::string> validate_custom_app(const Criteria& aInput)
{
    if (aInput.app.has_value() && aInput.app.value().mToken == SharedLibCriterion::kAppName)
    {
        return core::error_message_for_empty_parameter(plato::criteria::library::detail::criterion_name(aInput),
                                                       aInput.shared_library_path, "shared_library_path");
    }
    else
    {
        return std::nullopt;
    }
}
}  // namespace detail

}  // namespace plato::criteria::extension

#endif
