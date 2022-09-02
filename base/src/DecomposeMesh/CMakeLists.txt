set(PLATO_INCLUDES ${PLATO_INCLUDES} 
  ${CMAKE_SOURCE_DIR}/base/src/DecomposeMesh
  PARENT_SCOPE )

set(DecomposeMesh_sources Plato_DecomposeMesh.cpp)
set(DecomposeMesh_headers Plato_DecomposeMesh.hpp)
INCLUDE_DIRECTORIES(${PLATO_INCLUDES})
add_library(PlatoDecomposeMesh     ${DecomposeMesh_sources}     ${DecomposeMesh_headers}     )

target_link_libraries(PlatoDecomposeMesh ${Trilinos_LIBRARIES} )
set(ADD_PLATO_LIBRARIES ${ADD_PLATO_LIBRARIES} PlatoDecomposeMesh)

set(PLATO_LIBRARIES ${PLATO_LIBRARIES} ${ADD_PLATO_LIBRARIES} )
set(PLATO_LIBRARIES ${PLATO_LIBRARIES} ${ADD_PLATO_LIBRARIES} PARENT_SCOPE)

if( CMAKE_INSTALL_PREFIX )
  install( TARGETS PlatoDecomposeMesh EXPORT PlatoDecomposeMesh
           LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
           ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
           PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
  install( FILES ${DecomposeMesh_headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
endif()
