IF( DAKOTADRIVER )

set( LIB_NAMES )
set( LIB_NAME DakotaDriver )
set( LIB_NAMES ${LIB_NAMES} ${LIB_NAME} )

set( ${LIB_NAME}_SOURCES Plato_DakotaAppInterface.cpp Plato_DakotaDataMap.cpp Plato_DakotaAppInterfaceUtilities.cpp)
set( ${LIB_NAME}_HEADERS Plato_DakotaAppInterface.hpp Plato_DakotaDataMap.hpp Plato_DakotaDriver.hpp Plato_DakotaAppInterfaceUtilities.hpp)
add_compile_definitions(${Dakota_DEFINES})
                        
INCLUDE_DIRECTORIES(${PLATO_INCLUDES} SYSTEM ${Dakota_INCLUDE_DIRS})

FOREACH(PLATO_LIB ${LIB_NAMES})
  add_library( Plato${PLATO_LIB} ${${PLATO_LIB}_SOURCES} ${${PLATO_LIB}_HEADERS} )
  set( ADD_PLATO_LIBRARIES ${ADD_PLATO_LIBRARIES} Plato${PLATO_LIB} )
ENDFOREACH()

set(PLATO_LIBRARIES ${PLATO_LIBRARIES} ${ADD_PLATO_LIBRARIES} ${Dakota_LIBRARIES} ${Boost_LIBRARIES} PARENT_SCOPE)

if( CMAKE_INSTALL_PREFIX )
 FOREACH(PLATO_LIB ${LIB_NAMES})
  install( TARGETS Plato${PLATO_LIB} EXPORT Plato${PLATO_LIB}
           LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
           ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
           PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
  install( FILES ${${PLATO_LIB}_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
 ENDFOREACH()
endif() # end CMAKE_INSTALL_PREFIX if statement 

ENDIF() #end dakotadriver if statement 
