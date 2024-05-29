# copy_test_utility_data_files_and_convert_to_exodus
#   Copies all the files from ${FUNCTIONAL_TEST_DATA_DIR} to the build and install paths.
#   Runs txtexo on each file to convert them from git friendly ascii txt files of the meshes to binary exo. 
#   Creates two variables to refer to in the code FUNCTIONAL_TEST_DATA_BUILD_PATH and FUNCTIONAL_TEST_DATA_INSTALL_PATH.
macro( copy_test_utility_data_files_and_convert_to_exodus )

    set( FUNCTIONAL_TEST_DATA_BUILD_PATH "${CMAKE_CURRENT_BINARY_DIR}/test_data")
    set( FUNCTIONAL_TEST_DATA_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/test_data")

    file(GLOB COPY_FILES ${FUNCTIONAL_TEST_DATA_DIR}/*.txt)
    file(COPY ${COPY_FILES} DESTINATION "${FUNCTIONAL_TEST_DATA_BUILD_PATH}")
    install(FILES ${COPY_FILES} DESTINATION "${FUNCTIONAL_TEST_DATA_INSTALL_PATH}")
    
    foreach( TXTFILE ${COPY_FILES} )
        
        get_filename_component(BASE_FILE_NAME ${TXTFILE} NAME_WLE)
        set(FULLPATH_TXT_FILE "${FUNCTIONAL_TEST_DATA_DIR}/${BASE_FILE_NAME}.txt")
        set(BUILDPATH_EXO_FILE "${FUNCTIONAL_TEST_DATA_BUILD_PATH}/${BASE_FILE_NAME}.exo")
        set(INSTALLPATH_EXO_FILE "${FUNCTIONAL_TEST_DATA_INSTALL_PATH}/${BASE_FILE_NAME}.exo")
        execute_process(COMMAND "txtexo" ${FULLPATH_TXT_FILE} ${BUILDPATH_EXO_FILE} ) 
        execute_process(COMMAND "txtexo" ${FULLPATH_TXT_FILE} ${INSTALLPATH_EXO_FILE} ) 
        
    endforeach(TXTFILE)

endmacro(copy_test_utility_data_files_and_convert_to_exodus)
