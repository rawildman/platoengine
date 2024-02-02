include(${CMAKE_UTIL_DIR}/create_plato_library.cmake)
include(${CMAKE_UTIL_DIR}/create_plato_unittester.cmake)

set(LINK_LIBS STKIO::stk_io)
create_plato_library(PlatoFunctionalUtilities "." "${LINK_LIBS}")

if( UNIT_TESTING )
  
  set(LINK_LIBS PlatoFunctionalUtilities 
                PlatoFunctionalTestUtilities)

  create_plato_unittester(PlatoFunctionalUtilities_UnitTester "unittest" "${LINK_LIBS}")

endif()