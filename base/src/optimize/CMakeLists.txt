set(LIB_NAMES)

set(${LIB_NAME}_HEADERS Plato_BoundsBase.hpp
                        Plato_RestartFileUtilities.hpp
                        Plato_NullConstraint.hpp
                        Plato_OptimizerParser.hpp
                        Plato_DriverFactory.hpp
                        Plato_OptimizersIO_Utilities.hpp
                        Plato_CommWrapper.hpp
                        Plato_Types.hpp
                        Plato_Circle.hpp
                        Plato_HostBounds.hpp
                        Plato_Himmelblau.hpp
                        Plato_KokkosTypes.hpp
                        Plato_ErrorChecks.hpp
                        Plato_HessianLBFGS.hpp
                        Plato_DeviceBounds.hpp
                        Plato_OptimizersIO.hpp
                        Plato_GoldsteinPrice.hpp
                        Plato_StateData.hpp
                        Plato_StateManager.hpp
                        Plato_AlgebraFactory.hpp
                        Plato_EngineObjective.hpp
                        Plato_EngineConstraint.hpp
                        Plato_GradientOperator.hpp
                        Plato_GradientOperatorList.hpp
                        Plato_IdentityHessian.hpp
                        Plato_LinearOperator.hpp
                        Plato_LinearOperatorList.hpp
                        Plato_MultiVectorList.hpp
                        Plato_Preconditioner.hpp
                        Plato_Radius.hpp
                        Plato_Rosenbrock.hpp
                        Plato_AnalyticalHessian.hpp
                        Plato_AnalyticalGradient.hpp
                        Plato_Criterion.hpp
                        Plato_CriterionList.hpp
                        Plato_DataFactory.hpp
                        Plato_DistributedReductionOperations.hpp
                        Plato_LinearAlgebra.hpp
                        Plato_MultiVector.hpp
                        Plato_ReductionOperations.hpp
                        Plato_StandardMultiVector.hpp
                        Plato_StandardVector.hpp
                        Plato_StandardVectorReductionOperations.hpp
                        Plato_Vector.hpp
                        Plato_DriverInterface.hpp
                        Plato_SOParameterStudies.hpp
                        Plato_DistributedVector.hpp
                        Plato_EpetraSerialDenseVector.hpp
                        Plato_SOParameterStudiesInterface.hpp
                        Plato_EpetraSerialDenseMultiVector.hpp
                        Plato_LinearCriterionHessian.hpp
                        Plato_CentralDifferenceHessian.hpp
                        Plato_DakotaFactory.hpp
                        Plato_OptimizerFactory.hpp
                        Plato_OptimizerInterface.hpp
                        Plato_OptimizerUtilities.hpp
                        Plato_KokkosVector.hpp
                        Plato_KokkosReductionOperations.hpp
                        Plato_ROLInterface.hpp
                        Plato_StochasticROLInterface.hpp
                        Plato_SerialEpetraVectorROL.hpp
                        Plato_DistributedVectorROL.hpp
                        Plato_ReducedConstraintROL.hpp
                        Plato_ReducedObjectiveROL.hpp
                        Plato_ReducedStochasticObjectiveROL.hpp
                        Plato_ConstraintSimOptROL.hpp
                        Plato_ObjectiveSimOptROL.hpp
                        Plato_ProxyComplianceROL.hpp
                        Plato_SerialVectorROL.hpp
                        Plato_ProxyVolumeROL.hpp
                        )

INCLUDE_DIRECTORIES(${PLATO_INCLUDES})

FOREACH(PLATO_LIB ${LIB_NAMES})
  add_library(Plato${PLATO_LIB}     ${${PLATO_LIB}_SOURCES}     ${${PLATO_LIB}_HEADERS}     )
  set(ADD_PLATO_LIBRARIES ${ADD_PLATO_LIBRARIES} Plato${PLATO_LIB})
ENDFOREACH()
set(PLATO_LIBRARIES ${PLATO_LIBRARIES} ${ADD_PLATO_LIBRARIES} PARENT_SCOPE)

if( CMAKE_INSTALL_PREFIX )
 FOREACH(PLATO_LIB ${LIB_NAMES})
  install( TARGETS Plato${PLATO_LIB} EXPORT Plato${PLATO_LIB}
           LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
           ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
           PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
  install( FILES ${${PLATO_LIB}_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
 ENDFOREACH()
endif()
