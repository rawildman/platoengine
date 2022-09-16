###############################################################################
# Sources:
###############################################################################
SET(Plato_Interface_UnitTester_SRCS     Plato_Test_Interface_Boost.cpp
                                        Interface_UnitMain.cpp
                                        )

#SET(Plato_Interface_UnitTester_HDRS XMLGenerator_UnitTester.hpp)

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -Wno-unused-variable -Wno-write-strings")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused-variable -Wno-write-strings")
endif()

set(PLATOUNIT_INCLUDES ${CMAKE_SOURCE_DIR}/base/src/tools ..)
INCLUDE_DIRECTORIES(${PLATOUNIT_INCLUDES})

# The order of these libraries and the duplicates are necessary
set(Plato_Interface_UnitTester_LIBS GTest::GTest 
                                     PlatoApp
                                     PlatoInterface
                                     ${PLATO_LIBRARIES}
                                     ${Trilinos_LIBRARIES}
                                     ${Trilinos_TPL_LIBRARIES}
                                     ${Boost_LIBRARIES}
                                     )
 
add_executable(Plato_Interface_UnitTester ${Plato_Interface_UnitTester_SRCS})
target_link_libraries(Plato_Interface_UnitTester ${Plato_Interface_UnitTester_LIBS} ${Plato_Interface_UnitTester_LIBS})
add_test(NAME Plato_Interface_UnitTester COMMAND ${CMAKE_BINARY_DIR}/base/src/interface/unittest/Plato_Interface_UnitTester)
set_property(TEST Plato_Interface_UnitTester PROPERTY LABELS "small")

if( CMAKE_INSTALL_PREFIX )
  install( TARGETS Plato_Interface_UnitTester DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
endif()
