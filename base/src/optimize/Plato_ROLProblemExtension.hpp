/*
//@HEADER
// *************************************************************************
//   Plato Engine v.1.0: Copyright 2018, National Technology & Engineering
//                    Solutions of Sandia, LLC (NTESS).
//
// Under the terms of Contract DE-NA0003525 with NTESS,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Sandia Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact the Plato team (plato3D-help@sandia.gov)
//
// *************************************************************************
//@HEADER
*/

#pragma once

#include "ROL_Problem.hpp"
#include "ROL_Problem_Def.hpp"
#include "ROL_Bounds.hpp"
#include "ROL_Solver.hpp"


#include "ROL_Ptr.hpp"
#include "ROL_Types.hpp"
#include "ROL_ConstraintAssembler.hpp"
#include "ROL_SlacklessObjective.hpp"
#include "ROL_ReduceLinearConstraint.hpp"
#include "ROL_PolyhedralProjectionFactory.hpp"

namespace ROL
{
    
    template<typename Real>
    class ROLProblemExtension : Problem<Real>
    {

    public:
        ROLProblemExtension( const Ptr<Objective<Real>> &obj,
           const Ptr<Vector<Real>>    &x,
           const Ptr<Vector<Real>>    &g ):Problem(obj,x,g)
           {}

        void ROLProblemExtension<Real>::checkDerivatives(double aPerturbationScale, bool printToStream, std::ostream &outStream) const
        {
            const Real scale(aPerturbationScale);
            Ptr<Vector<Real>> x, d, v, g, c, w;
            // Objective check
            x = INPUT_xprim_->clone();
            x->randomize(0.0, scale); //-one,one);
            d = INPUT_xprim_->clone();
            d->randomize(0.0, scale); //-one,one);
            v = INPUT_xprim_->clone();
            v->randomize(0.0, scale); //-one,one);
            g = INPUT_xdual_->clone();
            g->randomize(0.0, scale); //-one,one);
            if (printToStream)
            {
                outStream << std::endl
                          << "  Check objective function" << std::endl
                          << std::endl;
            }
            INPUT_obj_->checkGradient(*x, *g, *d, printToStream, outStream);
            INPUT_obj_->checkHessVec(*x, *g, *d, printToStream, outStream);
            INPUT_obj_->checkHessSym(*x, *g, *d, *v, printToStream, outStream);

            // Constraint check
            for (auto it = INPUT_con_.begin(); it != INPUT_con_.end(); ++it)
            {
                c = it->second.residual->clone();
                c->randomize(-scale, scale);
                w = it->second.multiplier->clone();
                w->randomize(-scale, scale);
                if (printToStream)
                {
                    outStream << std::endl
                              << "  " << it->first << ": Check constraint function" << std::endl
                              << std::endl;
                }
                it->second.constraint->checkApplyJacobian(*x, *v, *c, printToStream, outStream);
                it->second.constraint->checkAdjointConsistencyJacobian(*w, *v, *x, printToStream, outStream);
                it->second.constraint->checkApplyAdjointHessian(*x, *w, *v, *g, printToStream, outStream);
            }

            // Linear constraint check
            for (auto it = INPUT_linear_con_.begin(); it != INPUT_linear_con_.end(); ++it)
            {
                c = it->second.residual->clone();
                c->randomize(-scale, scale);
                w = it->second.multiplier->clone();
                w->randomize(-scale, scale);
                if (printToStream)
                {
                    outStream << std::endl
                              << "  " << it->first << ": Check constraint function" << std::endl
                              << std::endl;
                }
                it->second.constraint->checkApplyJacobian(*x, *v, *c, printToStream, outStream);
                it->second.constraint->checkAdjointConsistencyJacobian(*w, *v, *x, printToStream, outStream);
                it->second.constraint->checkApplyAdjointHessian(*x, *w, *v, *g, printToStream, outStream);
            }
        }
    };

}