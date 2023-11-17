#ifndef PLATO_FUNCTIONAL_MASS_OBJECTIVE
#define PLATO_FUNCTIONAL_MASS_OBJECTIVE

#include <string>

namespace Plato::Functional
{
class MassObjective
{
   public:
    ///@brief Construct a new Mass Objective object
    ///
    ///@param aDensity
    ///@param aTarget
    MassObjective(const double aDensity, const double aTarget);

    ///@brief return the total mass of the mesh
    ///
    ///@param aMeshFileName
    ///@return double
    [[nodiscard]] double mass(std::string_view aMeshFileName) const;

    ///@brief return the number of nodes in the mesh
    ///
    ///@param aMeshFileName
    ///@return unsigned int
    [[nodiscard]] unsigned int numMeshNodes(std::string_view aMeshFileName) const;

   private:
    double mDensity = 1.0;
    double mTarget = 0.0;
};

}  // namespace Plato::Functional

#endif
