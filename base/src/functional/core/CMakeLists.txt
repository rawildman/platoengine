include(${CMAKE_UTIL_DIR}/create_plato_library.cmake)
include(${CMAKE_UTIL_DIR}/create_plato_unittester.cmake)

set(LINK_LIBS PlatoInputParser 
              PlatoFunctionalUtilities)

create_plato_library(PlatoFunctionalCore "." "${LINK_LIBS}")

if( UNIT_TESTING )
  # Build serial unit tester
  set(FOLDERS testutilities
              unittest
              )
  set(LINK_LIBS PlatoFunctionalCore
                PlatoFunctionalTestUtilities)

  create_plato_unittester(PlatoFunctionalCore_UnitTester "${FOLDERS}" "${LINK_LIBS}")

  set(FOLDERS testutilities
              parallel_unittest
              )
  create_plato_parallel_unittester(PlatoFunctionalCore_ParallelUnitTester "${FOLDERS}" "${LINK_LIBS}" 2)

endif()