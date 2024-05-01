# copy_test_utility_data_files
#   Copies all the files from ${FUNCTIONAL_TEST_DATA_DIR} to the build and install paths.
#   Creates two variables to refer to in the code FUNCTIONAL_TEST_DATA_BUILD_PATH and FUNCTIONAL_TEST_DATA_INSTALL_PATH.
macro( copy_test_utility_data_files )

    set( FUNCTIONAL_TEST_DATA_BUILD_PATH "${CMAKE_CURRENT_BINARY_DIR}/test_data")
    set( FUNCTIONAL_TEST_DATA_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/test_data")

    

    file(GLOB COPY_FILES ${FUNCTIONAL_TEST_DATA_DIR}/*)
    file(COPY ${COPY_FILES} DESTINATION "${FUNCTIONAL_TEST_DATA_BUILD_PATH}")
    install(FILES ${COPY_FILES} DESTINATION "${FUNCTIONAL_TEST_DATA_INSTALL_PATH}")
    
endmacro(copy_test_utility_data_files)
