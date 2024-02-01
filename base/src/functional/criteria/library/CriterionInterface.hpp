#ifndef PLATO_FUNCTIONAL_CRITERION_INTERFACE
#define PLATO_FUNCTIONAL_CRITERION_INTERFACE

#include <memory>
#include <string>
#include <vector>

#include "MeshProxy.hpp"

namespace Plato::Functional
{
static constexpr std::string_view kCreateCriterionFunctionName = "plato_create_criterion";

/// @brief Interface for implementing criteria loaded from shared libraries.
///
/// Any external criterion (objective or constraint) should inherit this interface
/// and implement plato_create_criterion.
class CriterionInterface
{
   public:
    CriterionInterface() = default;
    virtual ~CriterionInterface() = default;

    /// @note When implementing a constraint, the target value will be subtracted in the optimizer interface.
    /// A criterion that is a constraint should then just evaluate without considering any target value.
    virtual double value(const MeshProxy& aMeshProxy) const = 0;
    virtual std::vector<double> gradient(const MeshProxy& aMeshProxy) const = 0;

    CriterionInterface(const CriterionInterface&) = delete;
    CriterionInterface& operator=(const CriterionInterface&) = delete;
    CriterionInterface(CriterionInterface&&) = delete;
    CriterionInterface& operator=(CriterionInterface&&) = delete;
};

extern "C" std::unique_ptr<CriterionInterface> plato_create_criterion(const std::vector<std::string>& aFileNames);

}  // namespace Plato::Functional

#endif
