include(./cmake/plato_functional_setup.cmake)
plato_functional_setup()

include(./cmake/create_plato_library.cmake)

set(LINK_LIBS PlatoFunctionalGeometry 
              PlatoFunctionalCriteria 
              PlatoFunctionalOptimizer)

create_plato_library(PlatoFunctional "." "${LINK_LIBS}")

add_subdirectory(./utilities)
add_subdirectory(./core)
add_subdirectory(./geometry)
add_subdirectory(./filter)
add_subdirectory(./criteria)
add_subdirectory(./optimizer)
add_subdirectory(./main)

if( UNIT_TESTING )
  add_subdirectory(./testutilities)
  add_subdirectory(./integration_tests)
  add_subdirectory(./unittest)
endif()
