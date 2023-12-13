set(SRCS Test_Aggregate.cpp
         Test_Compose.cpp
         Test_Function.cpp
         Test_InputParser.cpp
         Test_JacobianColumnEvaluator.cpp
         Test_JacobianMultiplier.cpp
         Test_NamedType.cpp
         Test_Penalty.cpp
         Test_ROLHelpers.cpp
         Test_Rosenbrock.cpp
         Test_Validation.cpp
         UnitMain.cpp)

set(HDRS Utilities.hpp)

set(TEST_EXE PlatoFunctionalCore_UnitTester)
add_executable(${TEST_EXE} ${SRCS} ${HDRS})
target_link_libraries(${TEST_EXE} PRIVATE GTest::GTest PlatoFunctionalCore PlatoFunctionalTestUtilities)

add_test(NAME ${TEST_EXE} COMMAND ${TEST_EXE})
set_property(TEST ${TEST_EXE} PROPERTY LABELS "small")

install( TARGETS ${TEST_EXE} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
