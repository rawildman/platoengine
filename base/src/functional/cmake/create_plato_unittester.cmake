include(${CMAKE_UTIL_DIR}/add_to_srcs_and_hdrs.cmake)

function( create_plato_unittester TEST_EXE DIRECTORIES TARGET_LINK_LIST)

    unset(TEST_SRCS)
    unset(TEST_HDRS)

    foreach( curDir ${DIRECTORIES} )
        add_to_srcs_and_hdrs(${curDir} TEST_SRCS TEST_HDRS)
    endforeach(curDir)

    list(APPEND TEST_SRCS ${TEST_UNIT_MAIN_INCL})

    add_executable(${TEST_EXE} ${TEST_SRCS} ${TEST_HDRS})
    target_link_libraries( ${TEST_EXE} PRIVATE GTest::GTest ${TARGET_LINK_LIST})
    add_test(NAME ${TEST_EXE} COMMAND ${TEST_EXE})
    set_property(TEST ${TEST_EXE} PROPERTY LABELS "small")

    install( TARGETS ${TEST_EXE} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

endfunction(create_plato_unittester)