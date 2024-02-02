include(${CMAKE_UTIL_DIR}/create_plato_library.cmake)
include(${CMAKE_UTIL_DIR}/create_plato_unittester.cmake)

set(LINK_LIBS PlatoFunctionalCore 
              PlatoFunctionalLinearAlgebra
              PlatoFunctionalTestUtilities)

create_plato_library(PlatoFunctionalUnitTestUtilities testutilities "${LINK_LIBS}" )

# Build serial unit tester
set(LINK_LIBS ROL::rol 
              PlatoFunctional 
              PlatoFunctionalCore 
              PlatoInputParser 
              PlatoFunctionalUnitTestUtilities)

create_plato_unittester(PlatoFunctional_UnitTester "serial" "${LINK_LIBS}")

# Parallel tests
set(NUM_RANKS_FOR_TEST 4)
create_plato_parallel_unittester(PlatoFunctional_ParallelUnitTester "parallel" "${LINK_LIBS}" ${NUM_RANKS_FOR_TEST})
