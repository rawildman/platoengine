set(SRCS ROLPenaltyFunction.cpp
         Test_ConstraintValidationToFactory.cpp
         Test_GeometryValidationToFactory.cpp
         Test_PlatoProblem.cpp
         Test_ObjectiveValidationToFactory.cpp
         Test_OptimizeRosenbrock.cpp 
         Test_OptimizerValidationToFactory.cpp
         Test_ROLConstraint.cpp
         Test_ROLObjectiveRosenbrock.cpp
         Test_ROLPenaltyFunction.cpp
         Test_SimpleSumObjective.cpp
         Test_ValidatedInput.cpp
         UnitMain.cpp 
         )

set(HDRS ROLPenaltyFunction.hpp
         ROLTestUtilities.hpp)

set(TEST_EXE PlatoFunctional_UnitTester)
add_executable(${TEST_EXE} ${SRCS} ${HDRS})
target_link_libraries(${TEST_EXE} PRIVATE GTest::GTest ROL::rol PlatoFunctional PlatoFunctionalCore PlatoInputParser PlatoFunctionalTestUtilities)

add_test(NAME ${TEST_EXE} COMMAND ${TEST_EXE})
set_property(TEST ${TEST_EXE} PROPERTY LABELS "small")

install( TARGETS ${TEST_EXE} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
