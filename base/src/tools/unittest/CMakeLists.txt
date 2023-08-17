SET(SRCS UnitMain.cpp
         ../Plato_ParseCSMUtilities.cpp
         Plato_Test_InputData.cpp
         Plato_Test_Utilities.cpp
         Plato_Test_EnumTable.cpp
         Plato_Test_EngineDataParser.cpp)

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -Wno-unused-variable -Wno-write-strings")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused-variable -Wno-write-strings")
endif()

add_executable(PlatoTools_UnitTester ${SRCS})
target_link_libraries(PlatoTools_UnitTester GTest::GTest PlatoParser PlatoExceptions )
add_test(NAME PlatoTools_UnitTester COMMAND PlatoTools_UnitTester)
set_property(TEST PlatoTools_UnitTester PROPERTY LABELS "small")

install( TARGETS PlatoTools_UnitTester EXPORT PlatoEngine DESTINATION bin )
