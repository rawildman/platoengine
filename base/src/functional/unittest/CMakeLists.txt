include(${CMAKE_UTIL_DIR}/create_plato_library.cmake)
include(${CMAKE_UTIL_DIR}/create_plato_unittester.cmake)

# Build serial unit tester
set(LINK_LIBS ROL::rol 
                PlatoFunctional 
                PlatoFunctionalCore 
                PlatoInputParser 
                PlatoFunctionalTestUtilities)

create_plato_unittester(PlatoFunctional_UnitTester "serial;testutilities" "${LINK_LIBS}")

# Parallel tests
create_plato_parallel_unittester(PlatoFunctional_ParallelUnitTester "parallel;testutilities" "${LINK_LIBS}" 4)