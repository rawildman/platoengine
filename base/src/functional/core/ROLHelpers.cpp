#include "ROLHelpers.hpp"

namespace Plato::Functional
{
ROL::StdVector<double> operator*(double aAlpha, ROL::StdVector<double> aVec)
{
    aVec.scale(aAlpha);
    return aVec;
}

ROL::StdVector<double> operator+(const ROL::StdVector<double>& aVec1, ROL::StdVector<double> aVec2)
{
    aVec2.plus(aVec1);
    return aVec2;
}

ROL::Ptr<ROL::Vector<double>> copy_vector(const ROL::StdVector<double>& aVector)
{
    auto tCopy = ROL::makePtr<ROL::StdVector<double>>(aVector.dimension());
    for (int k = 0; k < aVector.dimension(); ++k)
    {
        (*tCopy)[k] = aVector[k];
    }
    return tCopy;
}

}  // namespace Plato::Functional
