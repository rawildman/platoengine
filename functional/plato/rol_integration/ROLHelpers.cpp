#include "plato/rol_integration/ROLHelpers.hpp"

namespace plato::rol_integration
{

ROL::StdVector<double> generate_perturbation(const int aDimension)
{
    ROL::StdVector<double> tPerturbation(aDimension);
    tPerturbation.randomize();
    return tPerturbation;
}

}  // namespace plato::rol_integration