include(${CMAKE_UTIL_DIR}/create_plato_library.cmake)
include(${CMAKE_UTIL_DIR}/create_plato_unittester.cmake)

set(FOLDERS library
            extension)

set(LINK_LIBS PlatoFunctionalCore
              PlatoFunctionalLinearAlgebra
              PlatoFunctionalUtilities
              PlatoInputParser)

create_plato_library(PlatoFunctionalFilter "${FOLDERS}" "${LINK_LIBS}")


if( UNIT_TESTING )
  # Build test filter shared lib
  add_subdirectory(./testutilities)

  # Build unit tester
  set(FOLDERS library/unittest
              extension/unittest)
  create_plato_unittester(PlatoFunctional_FilterUnitTester "${FOLDERS}" PlatoFunctionalFilter)

endif()