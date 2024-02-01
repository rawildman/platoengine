set(SRCS SharedLibIdentityFilter.cpp)
set(HDRS SharedLibIdentityFilter.hpp)

set(LIB_NAME PlatoIdentityFilter)
add_library(${LIB_NAME} SHARED ${SRCS} ${HDS} )
target_include_directories(${LIB_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/testutilities>)
target_link_libraries(${LIB_NAME} PUBLIC PlatoFunctionalFilter)

install( TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)