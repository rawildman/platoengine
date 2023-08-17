SET(SRCS ../Plato_InitializeValues.cpp
         ../Plato_HarvestDataFromFile.cpp
         Plato_Test_Operations.cpp
         Plato_Test_Operations_Boost.cpp
         Operations_UnitMain.cpp
         )

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -Wno-unused-variable -Wno-write-strings")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused-variable -Wno-write-strings")
endif()

add_executable(Plato_Operations_UnitTester ${SRCS})
target_link_libraries(Plato_Operations_UnitTester PlatoApp GTest::GTest Boost::serialization)

add_test(NAME Plato_Operations_UnitTester COMMAND Plato_Operations_UnitTester)
set_property(TEST Plato_Operations_UnitTester PROPERTY LABELS "small")

install( TARGETS Plato_Operations_UnitTester EXPORT PlatoEngine DESTINATION bin )
