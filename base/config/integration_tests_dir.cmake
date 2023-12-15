macro ( integration_tests_dir )

  if( NOT EXISTS ${INTEGRATION_TESTS_DIR} )
    set(INTEGRATION_TESTS_DIR ${PROJECT_SOURCE_DIR}/../plato-integration-tests )
  endif()
  if( NOT EXISTS ${INTEGRATION_TESTS_DIR} )
    message( WARNING "Cound not find plato-integration-tests directory, integration tests may not function." )
  else()

endmacro()
