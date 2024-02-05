set(SRCS InputParser.cpp
         JacobianColumnEvaluator.cpp
         ParameterBounds.cpp
         ValidationUtilities.cpp)

set(HDRS Aggregate.hpp
         BoundClusivity.hpp
         Compose.hpp
         DynamicVector.hpp
         DynamicVectorSerialization.hpp
         Function.hpp
         InputParser.hpp
         JacobianColumnEvaluator.hpp
         MeshProxy.hpp
         ParallelAggregate.hpp
         ParameterBounds.hpp
         ROLHelpers.hpp
         ValidatedInputTypeWrapper.hpp
         ValidationRegistration.hpp
         ValidationUtilities.hpp)

set(LIB_NAME PlatoFunctionalCore)
add_library(${LIB_NAME} ${SRCS} ${HDRS} )
target_include_directories(${LIB_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> )
target_link_libraries(${LIB_NAME} PUBLIC Boost::headers ROL::rol PlatoInputParser PlatoFunctionalUtilities)

install( TARGETS ${LIB_NAME} EXPORT PlatoEngine
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
target_include_directories(${LIB_NAME} INTERFACE $<INSTALL_INTERFACE:include>)
install( FILES ${HDRS} DESTINATION include)

if( UNIT_TESTING )
  add_subdirectory(./unittest)
endif()
