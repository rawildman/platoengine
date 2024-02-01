include(${CMAKE_UTIL_DIR}/create_plato_library.cmake)
include(${CMAKE_UTIL_DIR}/create_plato_unittester.cmake)

create_plato_library(PlatoFunctionalOptimizer "." "PlatoFunctionalCore;PlatoInputParser" )

if( UNIT_TESTING )
  # Build unit tester
  set(FOLDERS unittest)
  set(LINK_LIBS PlatoFunctionalOptimizer 
                PlatoFunctionalTestUtilities)

  create_plato_unittester(PlatoFunctional_OptimizerUnitTester "${FOLDERS}" "${LINK_LIBS}")

endif()
