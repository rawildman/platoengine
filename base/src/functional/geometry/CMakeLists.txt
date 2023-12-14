set(SRCS BrickShapeGeometry.cpp
         detail/GeometryRegistrationUtilities.cpp
         GeometryFactory.cpp
         GeometryRegistration.cpp
         GeometryValidation.cpp
         DensityTopology.cpp)

set(HDRS BrickShapeGeometry.hpp
         detail/GeometryRegistrationUtilities.hpp
         detail/GeometryInputBuilder.hpp
         GeometryFactory.hpp
         GeometryRegistration.hpp
         GeometryValidation.hpp
         DensityTopology.hpp)

set(LIB_NAME PlatoFunctionalGeometry)
add_library(${LIB_NAME} SHARED ${SRCS} ${HDRS} )
target_include_directories(${LIB_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> )
target_link_libraries(${LIB_NAME} PUBLIC PlatoFunctionalUtilities PlatoFunctionalCore PlatoFunctionalFilter PlatoInputParser)

install( TARGETS ${LIB_NAME} EXPORT PlatoEngine
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
target_include_directories(${LIB_NAME} INTERFACE $<INSTALL_INTERFACE:include>)
install( FILES ${HDRS} DESTINATION include)

if( UNIT_TESTING )
  add_subdirectory(./unittest)
endif()
