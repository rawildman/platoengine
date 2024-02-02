include(${CMAKE_UTIL_DIR}/create_plato_library.cmake)
include(${CMAKE_UTIL_DIR}/create_plato_unittester.cmake)

set(LINK_LIBS PlatoFunctionalCore 
              PlatoFunctionalTestUtilities)

create_plato_library(PlatoFunctionalUnitTestUtilities "testutilities" "${LINK_LIBS}" )

# Build serial unit tester
set(LINK_LIBS ROL::rol 
                PlatoFunctional 
                PlatoFunctionalCore 
                PlatoInputParser 
                PlatoFunctionalTestUtilities
                PlatoFunctionalUnitTestUtilities)

create_plato_unittester(PlatoFunctional_UnitTester "serial" "${LINK_LIBS}")

# Parallel tests
create_plato_parallel_unittester(PlatoFunctional_ParallelUnitTester "parallel" "${LINK_LIBS}" 4)