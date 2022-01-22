###############################################################################
# Sources:
###############################################################################
SET(BamG_UnitTester_SRCS UnitMain.cpp Plato_Test_BamG.cpp)

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -Wno-unused-variable -Wno-write-strings")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused-variable -Wno-write-strings")
endif()

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/base/src/geometry/BamG)

set(BamG_UnitTester_LIBS BamGlib ${GTEST_BOTH_LIBRARIES} ${Trilinos_LIBRARIES} ${Trilinos_TPL_LIBRARIES} )
add_executable(BamG_UnitTester ${BamG_UnitTester_SRCS})
target_link_libraries(BamG_UnitTester ${BamG_UnitTester_LIBS})
add_test(NAME BamG_UnitTester COMMAND ${CMAKE_BINARY_DIR}/base/src/geometry/BamG/unittest/BamG_UnitTester)

if( CMAKE_INSTALL_PREFIX )
  install( TARGETS BamG_UnitTester DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
endif()
