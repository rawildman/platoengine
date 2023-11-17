#ifndef PLATO_FUNCTIONAL_SHAREDLIBCRITERION
#define PLATO_FUNCTIONAL_SHAREDLIBCRITERION

#include <ROL_StdVector.hpp>
#include <filesystem>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "CriterionInterface.hpp"
#include "Function.hpp"
#include "MeshProxy.hpp"

namespace Plato::Functional
{

/// @brief for a criterion that is loaded from a shared library.
///
/// A shared library path is given on construction from which to load
/// a CriterionInterface object.
class SharedLibCriterion
{
   public:
    SharedLibCriterion(const std::filesystem::path& aSharedLibPath, const std::vector<std::string>& aFileNames);

    [[nodiscard]] double f(const MeshProxy& aMesh) const;

    [[nodiscard]] ROL::StdVector<double> df(const MeshProxy& aMesh) const;

   private:
    std::filesystem::path mSharedLibPath;
    std::shared_ptr<CriterionInterface> mCriterionFunction;
};

[[nodiscard]] auto make_shared_lib_function(const SharedLibCriterion& aSharedLibCriterion)
    -> Function<double, ROL::StdVector<double>, const MeshProxy&>;

}  // namespace Plato::Functional

#endif