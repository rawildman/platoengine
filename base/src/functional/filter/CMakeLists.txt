include(${CMAKE_UTIL_DIR}/create_plato_library.cmake)
include(${CMAKE_UTIL_DIR}/create_plato_unittester.cmake)

set(FOLDERS library
            extension)

set(LINK_LIBS PlatoFunctionalCore
              PlatoFunctionalUtilities
              PlatoInputParser)

create_plato_library(PlatoFunctionalFilter "${FOLDERS}" "${LINK_LIBS}")


if( UNIT_TESTING )
  # Build test filter shared lib
  add_subdirectory(./testutilities)

  # Build unit tester
  set(FOLDERS library/unittest
              extension/unittest)
  set(LINK_LIBS PlatoFunctionalFilter
                PlatoFunctionalTestUtilities)
  
  create_plato_unittester(PlatoFunctional_FilterUnitTester "${FOLDERS}" "${LINK_LIBS}")

endif()