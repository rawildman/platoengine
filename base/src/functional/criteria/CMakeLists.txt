set(SRCS ConstraintFactory.cpp
         CriterionFactory.cpp
         CriterionRegistration.cpp
         ObjectiveFactory.cpp
         NodalSumObjective.cpp
         SharedLibCriterion.cpp)

set(HDRS AffirmUtilities.hpp
         ConstraintFactory.hpp
         CriterionFactory.hpp
         CriterionInterface.hpp
         CriterionRegistration.hpp
         ObjectiveFactory.hpp
         NodalSumObjective.hpp
         SharedLibCriterion.hpp)

set(LIB_NAME PlatoFunctionalCriteria)
add_library(${LIB_NAME} SHARED ${SRCS} ${HDRS} )
target_include_directories(${LIB_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> )
target_link_libraries(${LIB_NAME} PUBLIC PlatoFunctionalCore PlatoFunctionalUtilities PlatoInputParser)

install( TARGETS ${LIB_NAME} EXPORT PlatoEngine
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
target_include_directories(${LIB_NAME} INTERFACE $<INSTALL_INTERFACE:include>)
install( FILES ${HDRS} DESTINATION include)

if( UNIT_TESTING )
  add_subdirectory(./unittest)
endif()
