set(SRCS PlatoProblem.cpp
         ROLConstraintFunction.cpp
         ROLObjectiveFunction.cpp )

set(HDRS PlatoProblem.hpp
         ROLConstraintFunction.hpp
         ROLObjectiveFunction.hpp )

add_library(PlatoFunctional ${SRCS} ${HDRS} )
target_include_directories(PlatoFunctional PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> )
target_link_libraries(PlatoFunctional PUBLIC PlatoFunctionalGeometry PlatoFunctionalCriteria PlatoFunctionalOptimizer)

install( TARGETS PlatoFunctional EXPORT PlatoEngine
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
target_include_directories(PlatoFunctional INTERFACE $<INSTALL_INTERFACE:include>)
install( FILES ${HDRS} DESTINATION include)

add_subdirectory(./utilities)
add_subdirectory(./core)
add_subdirectory(./geometry)
add_subdirectory(./filter)
add_subdirectory(./criteria)
add_subdirectory(./optimizer)
add_subdirectory(./main)

if( UNIT_TESTING )
  add_subdirectory(./testutilities)
  add_subdirectory(./integration_tests)
  add_subdirectory(./unittest)
endif()
