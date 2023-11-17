#include "ROLConstraintFunction.hpp"

namespace Plato::Functional
{

ROLConstraintFunction::ROLConstraintFunction(ConstraintFactory::Constraint<const ROL::StdVector<double>&> aConstraint)
    : mName(aConstraint.mName),
      mFunction(std::move(aConstraint.mConstraintFunction)),
      mConstraintTarget(aConstraint.mConstraintTarget),
      mLinear(aConstraint.mLinear)
{
}

void ROLConstraintFunction::value(ROL::Vector<double>& aConstraints, const ROL::Vector<double>& aControl, double&)
{
    auto tControlAsStdVector = dynamic_cast<const ROL::StdVector<double>&>(aControl);
    double tConstraintValue = mFunction.f(tControlAsStdVector);

    const double tOutput = tConstraintValue - mConstraintTarget;
    auto aConstraintsAsStdVector = dynamic_cast<const ROL::StdVector<double>&>(aConstraints);
    aConstraintsAsStdVector.getVector()->front() = tOutput;
}

void ROLConstraintFunction::applyJacobian(ROL::Vector<double>& aJacobianTimesDirection,
                                          const ROL::Vector<double>& aDirection,
                                          const ROL::Vector<double>& aControl,
                                          double& /*aTolerance*/)
{
    auto tControlAsStdVector = dynamic_cast<const ROL::StdVector<double>&>(aControl);
    const double tJacobianTimesDirection = mFunction.df(tControlAsStdVector).dot(aDirection);

    auto tJacobianTimesDirectionAsStdVector = dynamic_cast<const ROL::StdVector<double>&>(aJacobianTimesDirection);
    *tJacobianTimesDirectionAsStdVector.getVector() = {tJacobianTimesDirection};
}

void ROLConstraintFunction::applyAdjointJacobian(ROL::Vector<double>& aAdjointJacobianTimesDirection,
                                                 const ROL::Vector<double>& aDual,
                                                 const ROL::Vector<double>& aControl,
                                                 double& /*aTolerance*/)
{
    assert(aDual.dimension() == 1);
    assert(aAdjointJacobianTimesDirection.dimension() == aControl.dimension());
    auto tControlAsStdVector = dynamic_cast<const ROL::StdVector<double>&>(aControl);
    auto tDualAsStdVector = dynamic_cast<const ROL::StdVector<double>&>(aDual);

    auto tAdjointJacobianTimesDirection = mFunction.df(tControlAsStdVector);
    tAdjointJacobianTimesDirection.scale(tDualAsStdVector.getVector()->front());
    auto tAdjointJacobianTimesDirectionAsStdVector =
        dynamic_cast<const ROL::StdVector<double>&>(aAdjointJacobianTimesDirection);
    tAdjointJacobianTimesDirectionAsStdVector.set(tAdjointJacobianTimesDirection);
}

void ROLConstraintFunction::applyAdjointJacobian(ROL::Vector<double>& aAdjointJacobianTimesDirection,
                          const ROL::Vector<double>& aDual,
                          const ROL::Vector<double>& aControl,
                          const ROL::Vector<double>& /*aDualV*/,
                          double& aTolerance)
{
    applyAdjointJacobian(aAdjointJacobianTimesDirection, aDual, aControl, aTolerance);
}

bool ROLConstraintFunction::linear() const { return mLinear; }

const std::string& ROLConstraintFunction::name() const { return mName; }

}  // namespace Plato::Functional
