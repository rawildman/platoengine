include(${CMAKE_UTIL_DIR}/create_plato_library.cmake)
include(${CMAKE_UTIL_DIR}/create_plato_unittester.cmake)

set(FOLDERS library 
            extension)
set(LINK_LIBS PlatoFunctionalUtilities 
              PlatoFunctionalCore 
              PlatoFunctionalFilter 
              PlatoInputParser)

create_plato_library(PlatoFunctionalGeometry "${FOLDERS}" "${LINK_LIBS}")

if( UNIT_TESTING )
  # Build unit tester
  set(FOLDERS library/unittest 
              extension/unittest)
  set(LINK_LIBS PlatoFunctionalGeometry
                PlatoFunctionalTestUtilities)

  create_plato_unittester(PlatoFunctional_GeometryUnitTester "${FOLDERS}" "${LINK_LIBS}")

endif()