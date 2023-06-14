SET(PlatoOptimizerInterfaceUnitTester_SRCS UnitMain.cpp
                                           Plato_Test_ROLGradientCheck.cpp
                                           Plato_Test_ParseInterfaceOptimizerOptions.cpp)

set(PLATOUNIT_INCLUDES ${CMAKE_SOURCE_DIR}/base/src/optimize)
include_directories(${PLATOUNIT_INCLUDES})

set(PlatoOptimizerInterfaceUnitTester_LIBS GTest::GTest 
                                           PlatoTestUtilities
                                           PlatoParser
                                           PlatoInterface)

add_executable(OptimizerInterfaceUnitTester ${PlatoOptimizerInterfaceUnitTester_SRCS})
target_link_libraries(OptimizerInterfaceUnitTester ${PlatoOptimizerInterfaceUnitTester_LIBS})

add_test(NAME OptimizerInterfaceUnitTester COMMAND ${CMAKE_BINARY_DIR}/base/src/optimize/unittest/OptimizerInterfaceUnitTester)
set_property(TEST OptimizerInterfaceUnitTester PROPERTY LABELS "small")

if( CMAKE_INSTALL_PREFIX )
  install( TARGETS OptimizerInterfaceUnitTester DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
endif()
