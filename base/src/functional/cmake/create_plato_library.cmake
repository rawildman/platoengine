include(${CMAKE_UTIL_DIR}/add_to_srcs_and_hdrs.cmake)

function( create_plato_library LIBRARY_NAME DIRECTORIES TARGET_LINK_LIST)

    unset(LIB_SRCS)
    unset(LIB_HDRS)

    foreach( curDir ${DIRECTORIES} )
        add_to_srcs_and_hdrs(${curDir} LIB_SRCS LIB_HDRS)
    endforeach(curDir)

    add_library(${LIBRARY_NAME} SHARED ${LIB_SRCS} ${LIB_HDRS} )

    foreach( curDir ${DIRECTORIES} )
        target_include_directories(${LIBRARY_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${curDir}>)
    endforeach(curDir)

    target_link_libraries(${LIBRARY_NAME} PUBLIC ${TARGET_LINK_LIST})

    install( TARGETS ${LIBRARY_NAME} EXPORT PlatoEngine
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
    target_include_directories(${LIBRARY_NAME} INTERFACE $<INSTALL_INTERFACE:include>)
    install( FILES ${LIB_HDRS} DESTINATION include)

endfunction(create_plato_library)