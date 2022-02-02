###############################################################################
# Sources:
###############################################################################

SET(Prefix PlatoCLI)

SET(${Prefix}_UnitTester_SRCS PlatoAppCLI_UnitTester.cpp)

SET(PlatoXTK_UnitTester_HDRS )

set(PLATOUNIT_INCLUDES ${CMAKE_SOURCE_DIR}/apps/cli
                       ${CMAKE_SOURCE_DIR}/base/src/tools
                       ${CMAKE_SOURCE_DIR}/base/src/operations)

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -Wno-unused-variable -Wno-write-strings")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused-variable -Wno-write-strings")
endif()

INCLUDE_DIRECTORIES(${PLATOUNIT_INCLUDES})

set(${Prefix}_UnitTester_LIBS 
    ${GTEST_LIBRARY} 
    ${PLATO_LIBRARIES} 
    ${Trilinos_LIBRARIES} 
    ${Trilinos_TPL_LIBRARIES}
    ${Plato_EXTRA_LINK_FLAGS} )

add_executable(${Prefix}_UnitTester ${${Prefix}_UnitTester_SRCS} PlatoAppCLI_UnitTest_Main.cpp)
target_link_libraries(${Prefix}_UnitTester ${${Prefix}_UnitTester_LIBS})
add_test(NAME ${Prefix}_UnitTester COMMAND ${CMAKE_BINARY_DIR}/apps/cli/unittest/${Prefix}_UnitTester)

if( CMAKE_INSTALL_PREFIX )
  install( TARGETS ${Prefix}_UnitTester DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
endif()

###############################################################################
# END
###############################################################################