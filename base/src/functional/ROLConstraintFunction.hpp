#ifndef PLATO_FUNCTIONAL_ROLCONSTRAINT
#define PLATO_FUNCTIONAL_ROLCONSTRAINT

#include <ROL_Constraint.hpp>
#include <ROL_StdVector.hpp>

#include "ConstraintFactory.hpp"
#include "Function.hpp"

namespace Plato::Functional
{

class ROLConstraintFunction : public ROL::Constraint<double>
{
   public:
    using ROLPlatoFunction = Plato::Functional::Function<double, ROL::StdVector<double>, const ROL::StdVector<double>&>;

    ///@brief Construct a new ROLConstraintFunction object
    ///
    ///@param aConstraint
    ROLConstraintFunction(ConstraintFactory::Constraint<const ROL::StdVector<double>&> aConstraint);

    ///@brief Evaluate and populate aConstraints with the constraints at a given control vector and tolerance
    ///
    ///@param aConstraints
    ///@param aControl
    ///@param aTolerance
    void value(ROL::Vector<double>& aConstraints, const ROL::Vector<double>& aControl, double& aTolerance) override;

    ///@brief Evaluate the Jacobian in a given direction for the set of controls. Populate aJacobianTimesDirection
    ///
    ///@param aJacobianTimesDirection
    ///@param aDirection
    ///@param aControl
    ///@param aTolerance
    void applyJacobian(ROL::Vector<double>& aJacobianTimesDirection,
                       const ROL::Vector<double>& aDirection,
                       const ROL::Vector<double>& aControl,
                       double& aTolerance) override;

    ///@brief Evaluate the adjoint Jacobian times a direction for the set of controls. Populate
    ///aAdjointJacobianTimesDirection
    ///
    ///@param aAdjointJacobianTimesDirection
    ///@param aDual
    ///@param aControl
    ///@param aTolerance
    void applyAdjointJacobian(ROL::Vector<double>& aAdjointJacobianTimesDirection,
                              const ROL::Vector<double>& aDual,
                              const ROL::Vector<double>& aControl,
                              double& aTolerance) override;

    void applyAdjointJacobian(ROL::Vector<double>& aAdjointJacobianTimesDirection,
                              const ROL::Vector<double>& aDual,
                              const ROL::Vector<double>& aControl,
                              const ROL::Vector<double>& aDualV,
                              double& aTolerance) override;

    ///@brief is this constraint linear
    ///
    ///@return true the constraint is linear
    ///@return false
    bool linear() const;

    ///@brief return the name of the constraint for output
    ///
    ///@return const std::string&
    const std::string& name() const;

   private:
    std::string mName;
    ROLPlatoFunction mFunction;
    double mConstraintTarget = 0;
    bool mLinear = false;
};
}  // namespace Plato::Functional

#endif
