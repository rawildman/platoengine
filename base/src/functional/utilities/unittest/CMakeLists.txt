set(SRCS Test_StkGenerateMesh.cpp
         UnitMain.cpp )

set(HDRS )

set(TEST_EXE PlatoFunctional_UtilitiesUnitTester )
add_executable(${TEST_EXE} ${SRCS} ${HDRS})
target_link_libraries( ${TEST_EXE} PRIVATE GTest::GTest PlatoFunctionalUtilities PlatoFunctionalTestUtilities)

add_test(NAME ${TEST_EXE} COMMAND ${TEST_EXE})
set_property(TEST ${TEST_EXE} PROPERTY LABELS "small")

install( TARGETS ${TEST_EXE} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

# Parallel tests
set(PARALLEL_SRCS Test_RankSplitVector.cpp
                  ParallelUnitMain.cpp)

set(PARALLEL_TEST_EXE PlatoFunctionalUtilities_ParallelUnitTester)
add_executable(${PARALLEL_TEST_EXE} ${PARALLEL_SRCS} ${HDRS})
target_link_libraries(${PARALLEL_TEST_EXE} PRIVATE GTest::GTest PlatoFunctionalUtilities PlatoFunctionalTestUtilities)

set(MPI_RANKS_FOR_TEST 3)
add_test(NAME ${PARALLEL_TEST_EXE} COMMAND ${PARALLEL_TEST_EXE})
set_property(TEST ${PARALLEL_TEST_EXE} PROPERTY LABELS "small")
set_property(TEST ${PARALLEL_TEST_EXE} PROPERTY PROCESSORS ${MPI_RANKS_FOR_TEST})

install( TARGETS ${PARALLEL_TEST_EXE} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
