#include "plato/rol_integration/ROLHelpers.hpp"

namespace plato::rol_integration
{

ROL::StdVector<double> generate_perturbation(const int aDimension)
{
    ROL::StdVector<double> tPerturbation(aDimension);
    tPerturbation.randomize(-1, 1);
    tPerturbation.scale(1.0 / tPerturbation.norm());
    return tPerturbation;
}

}  // namespace plato::rol_integration
