include(${CMAKE_UTIL_DIR}/create_plato_library.cmake)
include(${CMAKE_UTIL_DIR}/create_plato_unittester.cmake)

set(LINK_LIBS PlatoFunctionalProcessManager)

create_plato_library(PlatoFunctional "." "${LINK_LIBS}")
