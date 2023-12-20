set(SRCS Test_OptimizerValidation.cpp
         UnitMain.cpp )

set(HDRS )

set(TEST_EXE PlatoFunctional_FactoryUnitTester)
add_executable(${TEST_EXE} ${SRCS} ${HDRS})
target_link_libraries( ${TEST_EXE} PRIVATE GTest::GTest ROL::rol PlatoFunctionalOptimizer PlatoFunctionalTestUtilities)

add_test(NAME ${TEST_EXE} COMMAND ${TEST_EXE})
set_property(TEST ${TEST_EXE} PROPERTY LABELS "small")

install( TARGETS ${TEST_EXE} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
