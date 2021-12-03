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

/*
 * Plato_DriverFactory.hpp
 *
 *  Created on: April 19, 2017
 *
 */

#pragma once

#include <mpi.h>

#include "Plato_DriverInterface.hpp"
#include "Plato_DiagnosticsInterface.hpp"
#include "Plato_ParticleSwarmEngineBCPSO.hpp"
#include "Plato_ParticleSwarmEngineALPSO.hpp"
#include "Plato_SOParameterStudiesInterface.hpp"
#include "Plato_OptimalityCriteriaInterface.hpp"
#include "Plato_MethodMovingAsymptotesEngine.hpp"
#include "Plato_KelleySachsBoundConstrainedInterface.hpp"
#include "Plato_KelleySachsAugmentedLagrangianInterface.hpp"
#include "Plato_GloballyConvergentMethodMovingAsymptotesInterface.hpp"

#ifdef ENABLE_ROL
#include "Plato_ROLAugmentedLagrangianInterface.hpp"
#include "Plato_ROLBoundConstrainedInterface.hpp"
#include "Plato_ROLLinearConstraintInterface.hpp"
#endif

#ifdef DAKOTADRIVER
#include "Plato_DakotaDriver.hpp"
#endif

namespace Plato
{

/******************************************************************************//**
 * \brief Construct interface to optimization algorithm
**********************************************************************************/
template<typename ScalarType, typename OrdinalType = size_t>
class DriverFactory
{
public:
    /******************************************************************************//**
     * \brief Constructor
    **********************************************************************************/
    DriverFactory()
    {
    }

    /******************************************************************************//**
     * \brief Destructuor
    **********************************************************************************/
    ~DriverFactory()
    {
    }

    /******************************************************************************//**
     * \brief Construct interface to optimization algorithm
     * \param [in] aInterface PLATO Engine interface
     * \param [in] aLocalComm local MPI communicator
     * \return non-const pointer to the optimization algorithm's interface
    **********************************************************************************/
    Plato::DriverInterface<ScalarType, OrdinalType>* create(Plato::Interface* aInterface, MPI_Comm aLocalComm)
    {
     Plato::DriverInterface<ScalarType, OrdinalType>* tDriver = nullptr;
     try {
       auto tInputData = aInterface->getInputData();

       if( tInputData.size<Plato::InputData>("Optimizer") > 1 )
       {
         Plato::ParsingException tParsingException("Plato::DriverFactory: multiple 'Optimizer' definitions");
         aInterface->registerException(tParsingException);
       }
       if( tInputData.size<Plato::InputData>("Optimizer") == 0 )
       {
         if( tInputData.size<Plato::InputData>("DakotaDriver") == 1 )
         {
#ifdef DAKOTADRIVER
           try {
             tDriver = new Plato::DakotaDriver<ScalarType, OrdinalType>(aInterface, aLocalComm);
             feclearexcept(FE_ALL_EXCEPT);
           } catch(...){aInterface->Catch(); tDriver = nullptr;}
           return tDriver;
#else
           Plato::ParsingException tParsingException("Plato::DriverFactory: plato was not compiled with dakota");
           aInterface->registerException(tParsingException);
#endif

         }
         else
         {
           Plato::ParsingException tParsingException("Plato::DriverFactory: missing 'Optimizer' definitions");
           aInterface->registerException(tParsingException);
         }
       }
       auto tOptNode = tInputData.get<Plato::InputData>("Optimizer");
       std::string tOptPackage = Plato::Get::String(tOptNode, "Package");
       if( tOptPackage == "OC" )
       {
         try {
           tDriver = new Plato::OptimalityCriteriaInterface<ScalarType, OrdinalType>(aInterface, aLocalComm);
         } catch(...){aInterface->Catch();}
       }
       else if( tOptPackage == "GCMMA" )
       {
         try {
           tDriver = new Plato::GloballyConvergentMethodMovingAsymptotesInterface<ScalarType, OrdinalType>(aInterface, aLocalComm);
         } catch(...){aInterface->Catch();}
       }
       else if( tOptPackage == "MMA" )
       {
         try {
           tDriver = new Plato::MethodMovingAsymptotesEngine<ScalarType, OrdinalType>(aInterface, aLocalComm);
         } catch(...){aInterface->Catch();}
       }
       else if( tOptPackage == "KSUC" )
       {
         try {
           Plato::driver::driver_t tType = Plato::driver::driver_t::KELLEY_SACHS_UNCONSTRAINED;
           tDriver = new Plato::KelleySachsBoundConstrainedInterface<ScalarType, OrdinalType>(aInterface, aLocalComm, tType);
         } catch(...){aInterface->Catch();}
       }
       else if( tOptPackage == "KSBC" )
       {
         try {
           Plato::driver::driver_t tType = Plato::driver::driver_t::KELLEY_SACHS_BOUND_CONSTRAINED;
           tDriver = new Plato::KelleySachsBoundConstrainedInterface<ScalarType, OrdinalType>(aInterface, aLocalComm, tType);
         } catch(...){aInterface->Catch();}
       }
       else if( tOptPackage == "KSAL" )
       {
         try {
           tDriver = new Plato::KelleySachsAugmentedLagrangianInterface<ScalarType, OrdinalType>(aInterface, aLocalComm);
         } catch(...){aInterface->Catch();}
       }
       else if( tOptPackage == "BCPSO" )
       {
         try {
           tDriver = new Plato::ParticleSwarmEngineBCPSO<ScalarType, OrdinalType>(aInterface, aLocalComm);
         } catch(...){aInterface->Catch();}
       }
       else if( tOptPackage == "ALPSO" )
       {
         try {
           tDriver = new Plato::ParticleSwarmEngineALPSO<ScalarType, OrdinalType>(aInterface, aLocalComm);
         } catch(...){aInterface->Catch();}
       }
#ifdef ENABLE_ROL
       else if( tOptPackage == "ROL AugmentedLagrangian" )
       {
         try {
           tDriver = new Plato::ROLAugmentedLagrangianInterface<ScalarType, OrdinalType>(aInterface, aLocalComm);
         } catch(...){aInterface->Catch();}
       }
       else if( tOptPackage == "ROL BoundConstrained" )
       {
         try {
           tDriver = new Plato::ROLBoundConstrainedInterface<ScalarType, OrdinalType>(aInterface, aLocalComm);
         } catch(...){aInterface->Catch();}
       }
       else if( tOptPackage == "ROL LinearConstraint" )
       {
         try {
           tDriver = new Plato::ROLLinearConstraintInterface<ScalarType, OrdinalType>(aInterface, aLocalComm);
         } catch(...){aInterface->Catch();}
       }
#endif
       else if( tOptPackage == "DerivativeChecker" )
       {
         try {
           tDriver = new Plato::DiagnosticsInterface<ScalarType, OrdinalType>(aInterface, aLocalComm);
         } catch(...){aInterface->Catch();}
       }
       else if( tOptPackage == "SOParameterStudies" )
       {
         try {
           tDriver = new Plato::SOParameterStudiesInterface<ScalarType, OrdinalType>(aInterface, aLocalComm);
         } catch(...){aInterface->Catch();}
       }
       else
       {
           std::stringstream tStringStream;
           tStringStream << "Plato::DriverFactory: " << tOptPackage << " Unknown." << std::endl
           << "Valid options are\n"
           << "\t OC ... Optimality Criteria\n"
           << "\t GCMMA ... Globally Convergent Method of Moving Asymptotes\n"
           << "\t MMA ... Method of Moving Asymptotes\n"
           << "\t KSUC ... Kelley Sachs Unconstrained\n"
           << "\t KSBC ... Kelley Sachs Bound Constrained\n"
           << "\t KSAL ... Kelley Sachs Augmented Lagrangian\n"
           << "\t BCPSO ... Bound Constrained Particle Swarm Optimization\n"
           << "\t ALPSO ... Augmented Lagrangian Particle Swarm Optimization\n"
           << "\t DerivativeChecker ... Derivative Checker Toolkit\n"
           << "\t SOParameterStudies ... Shape Optimization Parameter Study Toolkit\n"
#ifdef ENABLE_ROL
           << "\t ROL AugmentedLagrangian... Rapid Optimization Library Augmented Lagrangian\n"
           << "\t ROL BoundConstrained... Rapid Optimization Library Bound Constrained\n"
           << "\t ROL LinearConstraint... Rapid Optimization Library LinearConstraint\n"
#endif
#ifdef DAKOTADRIVER
           << "\t DakotaDriver... Plato driven by Dakota\n"
#endif
           << std::endl;
           throw Plato::ParsingException(tStringStream.str());
       }


     }
        catch(...)
        {
            aInterface->Catch();
            tDriver = nullptr;
        }

     return (tDriver);
   }

private:
    DriverFactory(const Plato::DriverFactory<ScalarType, OrdinalType>&);
    Plato::DriverFactory<ScalarType, OrdinalType> & operator=(const Plato::DriverFactory<ScalarType, OrdinalType>&);
};
// class DriverFactory

} // namespace Plato
