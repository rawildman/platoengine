include(${CMAKE_UTIL_DIR}/create_plato_library.cmake)

set(LINK_LIBS Kokkos::kokkos 
              PlatoInputParser)

create_plato_library(PlatoFunctionalTestUtilities "." "${LINK_LIBS}")