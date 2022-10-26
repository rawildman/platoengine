# specify codes
if(PLATOPROXY)

# Name the test with the directory name
Plato_new_test(TEST_NAME)

# Copy Input files from source to binary dir
set( testFiles "interface.xml;mpirun_save.source;mpirun_load.source")
Plato_add_test_files("${testFiles}")

file(READ ./mpirun_save.source SAVE_RUN_COMMAND)
file(READ ./mpirun_load.source LOAD_RUN_COMMAND)

# Add the test
add_test( NAME ${TEST_NAME}
          COMMAND ${CMAKE_COMMAND} 
          "-DTEST_COMMAND_SAVE=${SAVE_RUN_COMMAND}"
          "-DTEST_COMMAND_LOAD=${LOAD_RUN_COMMAND}"
          -DRESULT_FILE="plato_ksbc_algorithm_diagnostics.txt" 
          -DRESULT_FILE_SAVE="plato_ksbc_algorithm_diagnostics_save.txt" 
          -P ${CMAKE_SOURCE_DIR}/base/config/load_save_comparison.cmake )

set_property(TEST ${TEST_NAME} PROPERTY LABELS "small")

endif()
