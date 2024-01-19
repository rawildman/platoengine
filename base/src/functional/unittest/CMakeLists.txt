set(SRCS DynamicVectorPenaltyFunction.cpp
         Test_ConstraintValidationToFactory.cpp
         Test_GeometryValidationToFactory.cpp
         Test_PlatoProblem.cpp
         Test_ObjectiveValidationToFactory.cpp
         Test_OptimizeRosenbrock.cpp 
         Test_OptimizerValidationToFactory.cpp
         Test_ROLConstraint.cpp
         Test_ROLObjectiveRosenbrock.cpp
         Test_PenaltyFunction.cpp
         Test_Serialization.cpp
         Test_SimpleSumObjective.cpp
         Test_ValidatedInput.cpp
         UnitMain.cpp 
         )

set(HDRS DynamicVectorPenaltyFunction.hpp
         DynamicVectorTestUtilities.hpp)

set(TEST_EXE PlatoFunctional_UnitTester)
add_executable(${TEST_EXE} ${SRCS} ${HDRS})
target_link_libraries(${TEST_EXE} PRIVATE GTest::GTest ROL::rol PlatoFunctional PlatoFunctionalCore PlatoInputParser PlatoFunctionalTestUtilities)

add_test(NAME ${TEST_EXE} COMMAND ${TEST_EXE})
set_property(TEST ${TEST_EXE} PROPERTY LABELS "small")

install( TARGETS ${TEST_EXE} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

# Parallel tests
set(PARALLEL_SRCS Test_ParallelAggregateDynamicVector.cpp
                  ParallelUnitMain.cpp)

set(PARALLEL_TEST_EXE PlatoFunctional_ParallelUnitTester)
add_executable(${PARALLEL_TEST_EXE} ${PARALLEL_SRCS} ${HDRS})
target_link_libraries(${PARALLEL_TEST_EXE} PRIVATE GTest::GTest ROL::rol PlatoFunctional PlatoFunctionalCore PlatoInputParser PlatoFunctionalTestUtilities)

set(MPI_RANKS_FOR_TEST 2)
add_test(NAME ${PARALLEL_TEST_EXE} COMMAND ${PARALLEL_TEST_EXE})
set_property(TEST ${PARALLEL_TEST_EXE} PROPERTY LABELS "small")
set_property(TEST ${PARALLEL_TEST_EXE} PROPERTY PROCESSORS ${MPI_RANKS_FOR_TEST})

install( TARGETS ${PARALLEL_TEST_EXE} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
