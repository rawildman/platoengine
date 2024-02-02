include(${CMAKE_UTIL_DIR}/create_plato_library.cmake)
include(${CMAKE_UTIL_DIR}/create_plato_unittester.cmake)

create_plato_library(PlatoTestMassObjective "testutilities" "PlatoFunctionalCriteria")

# Build serial unit tester
set(LINK_LIBS TeuchosCore::teuchoscore 
              ROL::rol
              PlatoFunctional
              PlatoTestMassObjective)

create_plato_unittester(PlatoFunctional_IntegrationUnitTester serial "${LINK_LIBS}")

# Parallel tests
set(NUM_RANKS_FOR_TEST 2)
create_plato_parallel_unittester(PlatoFunctional_IntegrationParallelUnitTester parallel PlatoFunctional ${NUM_RANKS_FOR_TEST})
