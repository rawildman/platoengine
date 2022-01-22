###############################################################################
# Sources:
###############################################################################
SET(BamG_SRCS BamG.cpp)

SET(BamG_HDRS BamG.hpp)

add_library(BamGlib ${BamG_SRCS} ${BamG_HDRS})

add_executable(BamG BamG_main.cpp)
set(BamG_LIBS BamGlib ${Trilinos_LIBRARIES} ${Trilinos_TPL_LIBRARIES} )
target_link_libraries(BamG ${BamG_LIBS})

if( CMAKE_INSTALL_PREFIX )
  install( TARGETS BamG DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
  install( TARGETS BamGlib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
  install( FILES ${BamG_HDRS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
endif()

###############################################################################
###############################################################################


if( UNIT_TESTING )
  message( "-- Enabling unit testing " )
add_subdirectory(./unittest)
endif()
