###############################################################################
# Sources:
###############################################################################

SET(ExeName PlatoCLI)

SET(${ExeName}_SRCS Plato_AppCLI.cpp)

SET(${ExeName}_HDRS Plato_AppCLI.hpp)

INCLUDE_DIRECTORIES( ${PLATO_INCLUDES} )

# actual target:
set(${ExeName}_LIBS 
    ${PLATO_LIBRARIES} 
    ${Trilinos_LIBRARIES} 
    ${Trilinos_TPL_LIBRARIES}
    ${Plato_EXTRA_LINK_FLAGS})

# MPMD executable
add_executable(${ExeName} ${${ExeName}_SRCS} Plato_AppCLI_Main.cpp)
#set_target_properties(${ExeName} PROPERTIES NO_SYSTEM_FROM_IMPORTED true )
#add_library( PlatoAppCLI     ${${ExeName}_SRCS}     ${${ExeName}_HDRS} )
target_link_libraries(${ExeName} ${${ExeName}_LIBS})

# add PlatoCLI to Plato libraries
#set(PLATO_LIBRARIES ${PLATO_LIBRARIES} ${ExeName} )
#set(${ExeName}_BINARY ${CMAKE_BINARY_DIR}/apps/cli/${ExeName} PARENT_SCOPE)

if( CMAKE_INSTALL_PREFIX )
  install( TARGETS ${ExeName} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
endif()

if( UNIT_TESTING )
  add_subdirectory(./unittest)
endif()

###############################################################################
# END
###############################################################################
