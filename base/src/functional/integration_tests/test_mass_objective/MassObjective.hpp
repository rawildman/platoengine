#ifndef PLATO_FUNCTIONAL_MASS_OBJECTIVE
#define PLATO_FUNCTIONAL_MASS_OBJECTIVE

#include <string>

namespace plato::functional::integration_tests::test_mass_objective
{
class MassObjective
{
   public:
    ///@brief Construct a new Mass Objective object
    MassObjective(const double aDensity, const double aTarget);

    ///@brief return the total mass of the mesh
    [[nodiscard]] double mass(std::string_view aMeshFileName) const;

    ///@brief return the number of nodes in the mesh
    [[nodiscard]] unsigned int numMeshNodes(std::string_view aMeshFileName) const;

   private:
    double mDensity = 1.0;
    double mTarget = 0.0;
};

}  // namespace plato::functional::integration_tests::test_mass_objective

#endif
