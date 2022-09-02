###############################################################################
# Sources:
###############################################################################
SET(PlatoTools_UnitTester_SRCS UnitMain.cpp
                                   ../Plato_ParseCSMUtilities.cpp
                                   Plato_Test_InputData.cpp
                                   Plato_Test_Utilities.cpp)

SET(PlatoTools_UnitTester_HDRS )

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -Wno-unused-variable -Wno-write-strings")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused-variable -Wno-write-strings")
endif()

set(PLATOUNIT_INCLUDES ${CMAKE_SOURCE_DIR}/base/src/tools)
INCLUDE_DIRECTORIES(${PLATOUNIT_INCLUDES})

set(PlatoTools_UnitTester_LIBS GTest::GTest PlatoParser PlatoExceptions ${Trilinos_LIBRARIES} ${Trilinos_TPL_LIBRARIES} )
add_executable(PlatoTools_UnitTester ${PlatoTools_UnitTester_SRCS})
target_link_libraries(PlatoTools_UnitTester ${PlatoTools_UnitTester_LIBS})
add_test(NAME PlatoTools_UnitTester COMMAND ${CMAKE_BINARY_DIR}/base/src/tools/unittest/PlatoTools_UnitTester)
set_property(TEST PlatoTools_UnitTester PROPERTY LABELS "small")

if( CMAKE_INSTALL_PREFIX )
  install( TARGETS PlatoTools_UnitTester DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
endif()
