set(PlatoModels_UnitTester_SRCS 
  UnitMain.cpp
  Plato_Test_SIMP.cpp)

set(PLATOUNIT_INCLUDES ${CMAKE_SOURCE_DIR}/base/src/models)
include_directories(${PLATOUNIT_INCLUDES})

set(PlatoModels_UnitTester_LIBS GTest::GTest PlatoPenaltyModels PlatoParser PlatoExceptions)
add_executable(PlatoModels_UnitTester ${PlatoModels_UnitTester_SRCS})
target_link_libraries(PlatoModels_UnitTester PRIVATE ${PlatoModels_UnitTester_LIBS})
add_test(NAME PlatoModels_UnitTester COMMAND ${CMAKE_BINARY_DIR}/base/src/models/unittest/PlatoModels_UnitTester)
set_property(TEST PlatoModels_UnitTester PROPERTY LABELS "small")

if( CMAKE_INSTALL_PREFIX )
  install( TARGETS PlatoModels_UnitTester DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
endif()
