if( CMAKE_INSTALL_PREFIX )
  install( PROGRAMS plato-cli DESTINATION ${CMAKE_INSTALL_PREFIX}/etc )
  install( DIRECTORY esp DESTINATION ${CMAKE_INSTALL_PREFIX}/etc )
endif()

if( UNIT_TESTING )
  add_subdirectory(./esp/unittest)
endif()