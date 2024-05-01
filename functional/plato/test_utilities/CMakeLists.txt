include(${CMAKE_UTIL_DIR}/create_plato_library.cmake)

set(LINK_LIBS GTest::GTest
              Kokkos::kokkos 
              PlatoFunctionalInputParser)

configure_file(${FUNCTIONAL_BASE_DIR}/plato/cmake/TestDataFilePath.cpp ${CMAKE_CURRENT_BINARY_DIR}/configured_src/TestDataFilePath.cpp)
create_plato_library(PlatoFunctionalTestUtilities ".;${CMAKE_CURRENT_BINARY_DIR}/configured_src" "${LINK_LIBS}")
