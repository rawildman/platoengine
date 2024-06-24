include(${CMAKE_UTIL_DIR}/create_plato_library.cmake)
include(${CMAKE_UTIL_DIR}/create_plato_unittester.cmake)

set(LINK_LIBS PlatoFunctionalUtilities TpetraCore::tpetra)
create_plato_library(PlatoFunctionalTpetraIntegration "." "${LINK_LIBS}")
if( BUILD_WITH_SANITIZER_FLAGS )
  target_compile_definitions(PlatoFunctionalTpetraIntegration PRIVATE BUILD_WITH_SANITIZER_FLAGS=1)
else()
  target_compile_definitions(PlatoFunctionalTpetraIntegration PRIVATE BUILD_WITH_SANITIZER_FLAGS=0)
endif()

if( UNIT_TESTING )
  
  # Parallel tests
  set(LINK_LIBS PlatoFunctionalTpetraIntegration)
  set(NUM_RANKS_FOR_TEST 4)
  create_plato_parallel_integration_tester(PlatoFunctionalTpetraIntegration_ParallelUnitTester parallel_unittest "${LINK_LIBS}" ${NUM_RANKS_FOR_TEST})

endif()
