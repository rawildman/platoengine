
#pragma once

#include "Plato_Macros.hpp"
#include "Plato_DataFactory.hpp"
#include "Plato_OptimizersIO.hpp"
#include "Plato_MethodMovingAsymptotesCriterion.hpp"

#include "coin/IpIpoptApplication.hpp"
#include "coin/IpTNLP.hpp"

namespace Plato
{

using namespace Ipopt;

template<typename ScalarType, typename OrdinalType = size_t>
class IPOPT_MMA : public TNLP
{
public:
  IPOPT_MMA(std::shared_ptr<Plato::MultiVector<ScalarType, OrdinalType>> & aControlSolution,
            std::shared_ptr<Plato::MethodMovingAsymptotesCriterion<ScalarType, OrdinalType>> & aObjectiveMMA,
            std::vector<std::shared_ptr<Plato::MethodMovingAsymptotesCriterion<ScalarType, OrdinalType>>> & aConstraintsMMA,
            const std::shared_ptr<Plato::DataFactory<ScalarType, OrdinalType>> & aDataFactory) :
    mNumberOfControlVariables((*aControlSolution)[0].size()),
    mNumberOfConstraints(aConstraintsMMA.size()),
    mControlLowerBounds(mNumberOfControlVariables,  0.0),
    mControlUpperBounds(mNumberOfControlVariables,  1.0),
    mControlInitialGuess(mNumberOfControlVariables, 0.5),
    mControlSolution(aControlSolution),
    mTemporaryMultiVector1(aDataFactory->control().create()),
    mTemporaryMultiVector2(aDataFactory->control().create()),
    mMultiVectorOfOnes(aDataFactory->control().create()),
    mObjectiveMMA(aObjectiveMMA)
    {
        (*mMultiVectorOfOnes)[0].fill(1.0);
        assert(mControlSolution->getNumVectors() == static_cast<OrdinalType>(1));
        mConstraintsMMA.resize(mNumberOfConstraints);
        for (OrdinalType tIndex = 0; tIndex < mNumberOfConstraints; ++tIndex)
            mConstraintsMMA[tIndex] = aConstraintsMMA[tIndex];
    }

  virtual ~IPOPT_MMA() {}

  virtual bool get_nlp_info(Index& n, Index& m, Index& nnz_jac_g,
                            Index& nnz_h_lag, IndexStyleEnum& index_style)
  {
    n = mNumberOfControlVariables;

    m = mNumberOfConstraints;

    // The gradients of the constraints are dense
    nnz_jac_g = n * m;

    // The hessian of the lagrangian is diagonal for MMA
    nnz_h_lag = n;

    // use the C style indexing (0-based)
    index_style = TNLP::C_STYLE;

    return true;
  }

  virtual bool get_bounds_info(Index n, Number* x_l, Number* x_u,
                               Index m, Number* g_l, Number* g_u)
  {
    assert(n == (Index) mNumberOfControlVariables);
    assert(m == (Index) mNumberOfConstraints);

    for (Index tIndex = 0; tIndex < n; ++tIndex)
    {
        x_l[tIndex] = mControlLowerBounds[tIndex];
        x_u[tIndex] = mControlUpperBounds[tIndex];
    }

    for (Index constraint_number = 0; constraint_number < m; ++constraint_number){
        g_l[constraint_number] = -2.0e19;
        g_u[constraint_number] = 0.0;
    }

    return true;
  }

  virtual bool get_starting_point(Index n, bool init_x, Number* x,
                                  bool init_z, Number* z_L, Number* z_U,
                                  Index m, bool init_lambda,
                                  Number* lambda)
  {
    // Here, we assume we only have starting values for x
    // You can provide starting values for the dual variables if you wish
    assert(init_x == true);
    assert(init_z == false);
    assert(init_lambda == false);

    for (Index tIndex = 0; tIndex < n; ++tIndex)
        x[tIndex] = mControlInitialGuess[tIndex];

    return true;
  }

  virtual bool eval_f(Index n, const Number* x, bool new_x, Number& obj_value)
  {
      this->copyIpoptVectorIntoMultiVector(x, *mTemporaryMultiVector1);
      obj_value = mObjectiveMMA->value(*mTemporaryMultiVector1);
      return true;
  }

  virtual bool eval_grad_f(Index n, const Number* x, bool new_x, Number* grad_f)
  {
      this->copyIpoptVectorIntoMultiVector(x, *mTemporaryMultiVector1);
      mObjectiveMMA->gradient(*mTemporaryMultiVector1, *mTemporaryMultiVector2);
      this->copyMultiVectorIntoIpoptVector(*mTemporaryMultiVector2, grad_f);
      return true;
  }

  virtual bool eval_g(Index n, const Number* x, bool new_x, Index m, Number* g)
  {
      this->copyIpoptVectorIntoMultiVector(x, *mTemporaryMultiVector1);
      for (OrdinalType tIndex = 0; tIndex < mNumberOfConstraints; ++tIndex)
      {
          g[tIndex] = mConstraintsMMA[tIndex]->value(*mTemporaryMultiVector1);
      }
      return true;
  }

  virtual bool eval_jac_g(Index n, const Number* x, bool new_x,
                          Index m, Index nele_jac, Index* iRow, Index *jCol,
                          Number* values)
  {
    bool status = false;
    if (values == NULL) {
        // return the structure of the jacobian
        Index idx = 0;
        for (Index constraint_number = 0; constraint_number < m; ++constraint_number) {
            for (Index variable_number = 0; variable_number < n; ++variable_number) {
                iRow[idx] = constraint_number;
                jCol[idx] = variable_number;
                ++idx;
            }
        }
        status = true;
    }
    else {
        // return the values of the jacobian of the constraints
        this->copyIpoptVectorIntoMultiVector(x, *mTemporaryMultiVector1);
        Index tOffset = 0;
        for (Index tIndex = 0; tIndex < m; ++tIndex) {
            mConstraintsMMA[tIndex]->gradient(*mTemporaryMultiVector1, *mTemporaryMultiVector2);
            this->copyMultiVectorIntoIpoptVector(*mTemporaryMultiVector2, values, tOffset);
            tOffset += n;
        }
        status = true;
    }

    return status;
  }

  virtual bool eval_h(Index n, const Number* x, bool new_x,
                      Number obj_factor, Index m, const Number* lambda,
                      bool new_lambda, Index nele_hess, Index* iRow,
                      Index* jCol, Number* values)
  {
    if (values == NULL) {
        // Hessian of the lagrangian is diagonal for MMA
        assert(nele_hess == n);
        for (Index i = 0; i < n; ++i)
        {
            iRow[i] = i;
            jCol[i] = i;
        }
    }
    else {
        this->copyIpoptVectorIntoMultiVector(x, *mTemporaryMultiVector1);
        Index tOffset = 0;
        mObjectiveMMA->hessian(*mTemporaryMultiVector1, *mMultiVectorOfOnes, *mTemporaryMultiVector2);
        this->copyMultiVectorIntoIpoptVector(*mTemporaryMultiVector2, values, tOffset, obj_factor);
        for (Index tIndex = 0; tIndex < m; ++tIndex) {
            mConstraintsMMA[tIndex]->hessian(*mTemporaryMultiVector1, *mMultiVectorOfOnes, *mTemporaryMultiVector2);
            this->accumulateMultiVectorIntoIpoptVector(*mTemporaryMultiVector2, values, tOffset, lambda[tIndex]);
        }
    }
    return true;
  }

  virtual bool intermediate_callback(AlgorithmMode mode,
                                     Index iter, Number obj_value,
                                     Number inf_pr, Number inf_du,
                                     Number mu, Number d_norm,
                                     Number regularization_size,
                                     Number alpha_du, Number alpha_pr,
                                     Index ls_trials,
                                     const IpoptData* ip_data,
                                     IpoptCalculatedQuantities* ip_cq)
  {
      return true;
  }

  virtual void finalize_solution(SolverReturn status,
                                 Index n, const Number* x, const Number* z_L, const Number* z_U,
                                 Index m, const Number* g, const Number* lambda,
                                 Number obj_value,
				                         const IpoptData* ip_data,
				                         IpoptCalculatedQuantities* ip_cq)
  {
    for (Index tIndex = 0; tIndex < n; ++tIndex)
        (*mControlSolution)(0, tIndex) = x[tIndex];
  }

  void copyIpoptVectorIntoMultiVector(const Number* x, Plato::MultiVector<ScalarType, OrdinalType> & aMultiVector)
  {
      assert(aMultiVector.getNumVectors() == static_cast<OrdinalType>(1));
      for (OrdinalType tIndex = 0; tIndex < mNumberOfControlVariables; ++tIndex)
      {
          aMultiVector(0, tIndex) = x[tIndex];
      }
  }

  void copyMultiVectorIntoIpoptVector(Plato::MultiVector<ScalarType, OrdinalType> & aMultiVector,
                                      Number* x,
                                      Index aOffset = 0,
                                      ScalarType aScaleFactor = 1.0)
  {
      assert(aMultiVector.getNumVectors() == static_cast<OrdinalType>(1));
      for (OrdinalType tIndex = 0; tIndex < mNumberOfControlVariables; ++tIndex)
      {
          x[tIndex + aOffset] = aScaleFactor * aMultiVector(0, tIndex);
      }
  }

  void accumulateMultiVectorIntoIpoptVector(Plato::MultiVector<ScalarType, OrdinalType> & aMultiVector,
                                      Number* x,
                                      Index aOffset = 0,
                                      ScalarType aScaleFactor = 1.0)
  {
      assert(aMultiVector.getNumVectors() == static_cast<OrdinalType>(1));
      for (OrdinalType tIndex = 0; tIndex < mNumberOfControlVariables; ++tIndex)
      {
          x[tIndex + aOffset] += aScaleFactor * aMultiVector(0, tIndex);
      }
  }

  void setInitialGuess(const Plato::MultiVector<ScalarType, OrdinalType> & aInput)
  {
      assert(aInput.getNumVectors() == static_cast<OrdinalType>(1));
      for (OrdinalType tIndex = 0; tIndex < mNumberOfControlVariables; ++tIndex)
      {
          mControlInitialGuess[tIndex] = aInput(0, tIndex);
      }
  }

  void setControlLowerBounds(const Plato::MultiVector<ScalarType, OrdinalType> & aInput)
  {
      assert(aInput.getNumVectors() == static_cast<OrdinalType>(1));
      for (OrdinalType tIndex = 0; tIndex < mNumberOfControlVariables; ++tIndex)
      {
          mControlLowerBounds[tIndex] = aInput(0, tIndex);
      }
  }

  void setControlUpperBounds(const Plato::MultiVector<ScalarType, OrdinalType> & aInput)
  {
      assert(aInput.getNumVectors() == static_cast<OrdinalType>(1));
      for (OrdinalType tIndex = 0; tIndex < mNumberOfControlVariables; ++tIndex)
      {
          mControlUpperBounds[tIndex] = aInput(0, tIndex);
      }
  }

private:
  OrdinalType mNumberOfControlVariables;
  OrdinalType mNumberOfConstraints;

  std::vector<ScalarType> mControlLowerBounds;
  std::vector<ScalarType> mControlUpperBounds;
  std::vector<ScalarType> mControlInitialGuess;
  std::shared_ptr<Plato::MultiVector<ScalarType, OrdinalType>> mControlSolution;
  std::shared_ptr<Plato::MultiVector<ScalarType, OrdinalType>> mTemporaryMultiVector1;
  std::shared_ptr<Plato::MultiVector<ScalarType, OrdinalType>> mTemporaryMultiVector2;
  std::shared_ptr<Plato::MultiVector<ScalarType, OrdinalType>> mMultiVectorOfOnes;

  std::shared_ptr<Plato::MethodMovingAsymptotesCriterion<ScalarType, OrdinalType>> mObjectiveMMA;
  std::vector<std::shared_ptr<Plato::MethodMovingAsymptotesCriterion<ScalarType, OrdinalType>>> mConstraintsMMA;

}; // class IPOPT_MMA

template<typename ScalarType, typename OrdinalType = size_t>
class IpoptMMASubproblemSolver
{
public:
    /******************************************************************************//**
     * @brief Default constructor
     * @param [in] aObjective interface to objective function
     * @param [in] aConstraints interface to inequality constraints
     * @param [in] aDataFactory factory for linear algebra data structure
    **********************************************************************************/
    IpoptMMASubproblemSolver(std::shared_ptr<Plato::MethodMovingAsymptotesCriterion<ScalarType, OrdinalType>> & aObjectiveMMA,
                        std::vector<std::shared_ptr<Plato::MethodMovingAsymptotesCriterion<ScalarType, OrdinalType>>> & aConstraintsMMA,
                        const std::shared_ptr<Plato::DataFactory<ScalarType, OrdinalType>> & aDataFactory) :
                        mMaxNumSubproblemIterations(500),
                        mOptimalityTolerance(1.0e-7),
                        mFeasibilityTolerance(1.0e-5),
                        mSubproblemSolution(aDataFactory->control().create())
    {
        this->initialize(aObjectiveMMA, aConstraintsMMA, aDataFactory);
    }

    /******************************************************************************//**
     * @brief Default destructor
    **********************************************************************************/
    ~IpoptMMASubproblemSolver()
    {
    }

    /******************************************************************************//**
     * @brief Enable output of diagnostics (i.e. optimization problem status)
    **********************************************************************************/
    void enableDiagnostics()
    {
        mIpoptApp->Options()->SetIntegerValue("file_print_level", 5);
        mIpoptApp->Options()->SetStringValue("print_user_options", "yes");
        mIpoptApp->Options()->SetStringValue("print_timing_statistics", "yes");
        mIpoptApp->Options()->SetStringValue("output_file", "ipopt_mma_subproblem_diagnostics.txt");

        ApplicationReturnStatus tStatus;
        tStatus = mIpoptApp->Initialize();
        assert(tStatus == Solve_Succeeded);
    }

    /******************************************************************************//**
     * @brief Set maximum number of subproblem iterations
     * @param [in] aInput maximum number of subproblem iterations
    **********************************************************************************/
    void setMaxNumSubproblemIterations(const OrdinalType & aInput)
    {
        mMaxNumSubproblemIterations = aInput;
        mIpoptApp->Options()->SetIntegerValue("max_iter", mMaxNumSubproblemIterations);
        ApplicationReturnStatus tStatus;
        tStatus = mIpoptApp->Initialize();
        assert(tStatus == Solve_Succeeded);
    }

    /******************************************************************************//**
     * @brief Set feasibility stopping tolerance
     * @param [in] aInput feasibility stopping tolerance
    **********************************************************************************/
    void setFeasibilityTolerance(const ScalarType & aInput)
    {
        mFeasibilityTolerance = aInput;
        mIpoptApp->Options()->SetNumericValue("constr_viol_tol", mFeasibilityTolerance);
        ApplicationReturnStatus tStatus;
        tStatus = mIpoptApp->Initialize();
        assert(tStatus == Solve_Succeeded);
    }

    /******************************************************************************//**
     * @brief Set control initial guess
     * @param [in] aInput initial guess
    **********************************************************************************/
    void setInitialGuess(const Plato::MultiVector<ScalarType, OrdinalType> & aInput)
    {
        mSubProblem->setInitialGuess(aInput);
    }

    /******************************************************************************//**
     * @brief Set control lower bounds
     * @param [in] aInput lower bounds
    **********************************************************************************/
    void setControlLowerBounds(const Plato::MultiVector<ScalarType, OrdinalType> & aInput)
    {
        mSubProblem->setControlLowerBounds(aInput);
    }

    /******************************************************************************//**
     * @brief Set control upper bounds
     * @param [in] aInput upper bounds
    **********************************************************************************/
    void setControlUpperBounds(const Plato::MultiVector<ScalarType, OrdinalType> & aInput)
    {
        mSubProblem->setControlUpperBounds(aInput);
    }

    /******************************************************************************//**
     * @brief Get control solution
     * param [in/out] aSolution solution to optimization problem
    **********************************************************************************/
    void getSolution(Plato::MultiVector<ScalarType, OrdinalType> &aSolution) const
    {
        Plato::update(static_cast<ScalarType>(1.0), *mSubproblemSolution, static_cast<ScalarType>(0.0), aSolution);
    }

    /******************************************************************************//**
     * @brief Solve optimization subproblem
    **********************************************************************************/
    void solve()
    {
        mIpoptApp->OptimizeTNLP(mSubProblem);
    }

private:
    /******************************************************************************//**
     * @brief Initialize default parameters
     * @param [in] aObjectiveMMA interface to objective function
     * @param [in] aConstraintsMMA interface to inequality constraints
     * @param [in] aDataFactory factory for linear algebra data structure
    **********************************************************************************/
    void initialize(std::shared_ptr<Plato::MethodMovingAsymptotesCriterion<ScalarType, OrdinalType>> & aObjectiveMMA,
                    std::vector<std::shared_ptr<Plato::MethodMovingAsymptotesCriterion<ScalarType, OrdinalType>>> & aConstraintsMMA,
                    const std::shared_ptr<Plato::DataFactory<ScalarType, OrdinalType>> & aDataFactory)
    {
        mSubProblem = new Plato::IPOPT_MMA<ScalarType, OrdinalType>(mSubproblemSolution, aObjectiveMMA, aConstraintsMMA, aDataFactory);

        mIpoptApp = IpoptApplicationFactory();
        mIpoptApp->RethrowNonIpoptException(true);
        mIpoptApp->Options()->SetNumericValue("tol", mOptimalityTolerance);
        mIpoptApp->Options()->SetIntegerValue("max_iter", mMaxNumSubproblemIterations);
        mIpoptApp->Options()->SetNumericValue("constr_viol_tol", mFeasibilityTolerance);
        mIpoptApp->Options()->SetIntegerValue("print_level", 0);
        mIpoptApp->Options()->SetIntegerValue("file_print_level", 0); // to enable output, call enableDiagnostics method above
        mIpoptApp->Options()->SetIntegerValue("mumps_pivot_order", 0);

        // Derivative Checker for MMA Criteria | can check gradients and hessians
        // const bool check_derivatives_numerically = true;
        // if (check_derivatives_numerically)
        // {
        //     //mIpoptApp->Options()->SetStringValue("derivative_test", "first-order");
        //     mIpoptApp->Options()->SetStringValue("derivative_test", "second-order");
        //     //mIpoptApp->Options()->SetStringValue("derivative_test", "only-second-order");
        //     //mIpoptApp->Options()->SetStringValue("derivative_test_print_all", "yes");
        //     mIpoptApp->Options()->SetNumericValue("derivative_test_perturbation", 1.0e-4);
        //     mIpoptApp->Options()->SetNumericValue("point_perturbation_radius", 0.0);
        // }

        ApplicationReturnStatus tStatus;
        tStatus = mIpoptApp->Initialize();
        assert(tStatus == Solve_Succeeded);
    }


private:
    OrdinalType mMaxNumSubproblemIterations;
    ScalarType mOptimalityTolerance;
    ScalarType mFeasibilityTolerance;
    std::shared_ptr<Plato::MultiVector<ScalarType, OrdinalType>> mSubproblemSolution;

    SmartPtr<Plato::IPOPT_MMA<ScalarType, OrdinalType>> mSubProblem;
    SmartPtr<IpoptApplication> mIpoptApp;
};
// class IpoptMMASubproblemSolver



} // namespace Plato
