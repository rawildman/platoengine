include(${CMAKE_UTIL_DIR}/create_plato_library.cmake)
include(${CMAKE_UTIL_DIR}/create_plato_unittester.cmake)

set(LINK_LIBS STKIO::stk_io STKSearch::stk_search )
create_plato_library(PlatoFunctionalUtilities "." "${LINK_LIBS}")
if( BUILD_WITH_SANITIZER_FLAGS )
  target_compile_definitions(PlatoFunctionalUtilities PRIVATE BUILD_WITH_SANITIZER_FLAGS=1)
else()
  target_compile_definitions(PlatoFunctionalUtilities PRIVATE BUILD_WITH_SANITIZER_FLAGS=0)
endif()

if( UNIT_TESTING )
  create_plato_unittester(PlatoFunctionalUtilities unittest)
endif()
