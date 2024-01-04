set(SRCS Test_ConstraintFactory.cpp
         Test_CriterionValidation.cpp
         Test_ConstraintValidation.cpp
         Test_NodalSumObjective.cpp
         Test_ObjectiveValidation.cpp
         Test_Registration.cpp
         UnitMain.cpp )

set(HDRS )

set(TEST_EXE PlatoFunctional_CriteriaUnitTester )
add_executable(${TEST_EXE} ${SRCS} ${HDRS})
target_link_libraries( ${TEST_EXE} PRIVATE GTest::GTest PlatoFunctionalCriteria PlatoFunctionalTestUtilities)

add_test(NAME ${TEST_EXE} COMMAND ${TEST_EXE})
set_property(TEST ${TEST_EXE} PROPERTY LABELS "small")

install( TARGETS ${TEST_EXE} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )