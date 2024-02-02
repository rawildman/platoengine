# Build test objective shared lib
include(${CMAKE_UTIL_DIR}/create_plato_library.cmake)
include(${CMAKE_UTIL_DIR}/create_plato_unittester.cmake)

create_plato_library(PlatoTestMassObjective "testutilities" "PlatoFunctionalCriteria")

# Build serial unit tester
  set(LINK_LIBS TeuchosCore::teuchoscore 
                ROL::rol
                PlatoFunctional
                PlatoTestMassObjective
                PlatoFunctionalTestUtilities)

  create_plato_unittester(PlatoFunctional_IntegrationUnitTester "serial" "${LINK_LIBS}")

# Parallel tests
set(LINK_LIBS   PlatoFunctional
                PlatoFunctionalTestUtilities)

create_plato_parallel_unittester(PlatoFunctional_IntegrationParallelUnitTester "parallel" "${LINK_LIBS}" 2)                
set(PARALLEL_SRCS Test_ParallelObjectiveFactory.cpp
                  ParallelUnitMain.cpp)