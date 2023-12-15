set(SRCS Test_BrickShapeGeometry.cpp
         Test_BrickShapeGeometryValidation.cpp
         Test_GeometryFactory.cpp
         Test_GeometryRegistration.cpp
         Test_GeometryRegistrationUtilities.cpp
         Test_GeometryInputBuilder.cpp
         Test_GeometryValidation.cpp
         Test_DensityTopology.cpp
         Test_DensityTopologyValidation.cpp
         UnitMain.cpp )

set(HDRS )

set(TEST_EXE PlatoFunctional_GeometryUnitTester )
add_executable(${TEST_EXE} ${SRCS} ${HDRS})
target_link_libraries( ${TEST_EXE} PRIVATE GTest::GTest PlatoFunctionalGeometry PlatoFunctionalTestUtilities) 

add_test(NAME ${TEST_EXE} COMMAND ${TEST_EXE})
set_property(TEST ${TEST_EXE} PROPERTY LABELS "small")

install( TARGETS ${TEST_EXE} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
