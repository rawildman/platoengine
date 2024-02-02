include(${CMAKE_UTIL_DIR}/create_plato_library.cmake)
include(${CMAKE_UTIL_DIR}/create_plato_unittester.cmake)

set(DIRECTORIES library
                extension)
set(LINK_LIBS PlatoFunctionalCore
              PlatoFunctionalLinearAlgebra)
create_plato_library(PlatoFunctionalCriteria "${DIRECTORIES}" "${LINK_LIBS}")

if( UNIT_TESTING )
  set(DIRECTORIES library/unittest
                  extension/unittest)
  create_plato_unittester(PlatoFunctional_CriteriaUnitTester "${DIRECTORIES}" PlatoFunctionalCriteria)

  set(DIRECTORIES library/parallel_unittest)
  set(NUM_RANKS_FOR_TEST 3)
  create_plato_parallel_unittester(PlatoFunctional_CriteriaParallelUnitTester "${DIRECTORIES}" PlatoFunctionalCriteria ${NUM_RANKS_FOR_TEST})
endif()
