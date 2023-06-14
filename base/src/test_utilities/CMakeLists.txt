set(LIB_NAME TestUtilities)

set(${LIB_NAME}_SOURCES InterfaceTestFixture.cpp)
set(${LIB_NAME}_HEADERS InterfaceTestFixture.hpp)

add_library(Plato${LIB_NAME} ${${LIB_NAME}_SOURCES} ${${LIB_NAME}_HEADERS})

target_include_directories(Plato${LIB_NAME} PRIVATE ${PLATO_INCLUDES})

target_link_libraries(Plato${LIB_NAME} LINK_PUBLIC GTest::GTest Trilinos::all_selected_libs)

if( CMAKE_INSTALL_PREFIX )
  install( TARGETS Plato${LIB_NAME} EXPORT Plato${LIB_NAME}
           LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
           ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
           PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
endif()
