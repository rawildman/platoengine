set(SRCS Test_Aggregate.cpp
         Test_Compose.cpp
         Test_DynamicVector.cpp
         Test_Function.cpp
         Test_InputParser.cpp
         Test_JacobianColumnEvaluator.cpp
         Test_JacobianMultiplier.cpp
         Test_NamedType.cpp
         Test_ParameterBounds.cpp
         Test_Penalty.cpp
         Test_ROLHelpers.cpp
         Test_Rosenbrock.cpp
         Test_Validation.cpp
         Test_ValidationErrorCheckingUtilities.cpp
         UnitMain.cpp)

set(HDRS Utilities.hpp)

set(TEST_EXE PlatoFunctionalCore_UnitTester)
add_executable(${TEST_EXE} ${SRCS} ${HDRS})
target_link_libraries(${TEST_EXE} PRIVATE GTest::GTest PlatoFunctionalCore PlatoFunctionalTestUtilities)

add_test(NAME ${TEST_EXE} COMMAND ${TEST_EXE})
set_property(TEST ${TEST_EXE} PROPERTY LABELS "small")

install( TARGETS ${TEST_EXE} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

# Parallel tests
set(PARALLEL_SRCS Test_ParallelAggregate.cpp
                  UnitMain.cpp)

set(PARALLEL_TEST_EXE PlatoFunctionalCore_ParallelUnitTester)
add_executable(${PARALLEL_TEST_EXE} ${PARALLEL_SRCS} ${HDRS})
target_link_libraries(${PARALLEL_TEST_EXE} PRIVATE GTest::GTest PlatoFunctionalCore PlatoFunctionalTestUtilities)

set(MPI_RANKS_FOR_TEST 2)
add_test(NAME ${PARALLEL_TEST_EXE} COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} ${MPI_RANKS_FOR_TEST} ${MPIEXEC_PREFLAGS} ${PARALLEL_TEST_EXE} ${MPIEXEC_POSTFLAGS})
set_property(TEST ${PARALLEL_TEST_EXE} PROPERTY LABELS "small")
set_property(TEST ${PARALLEL_TEST_EXE} PROPERTY PROCESSORS ${MPI_RANKS_FOR_TEST})

install( TARGETS ${PARALLEL_TEST_EXE} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
