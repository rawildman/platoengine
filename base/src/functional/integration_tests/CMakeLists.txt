# Build test objective shared lib
set(SRCS MassObjective.cpp
         MassObjectiveInterface.cpp)
set(HDRS MassObjective.hpp
         MassObjectiveInterface.hpp)

set(LIB_NAME PlatoTestMassObjective)
add_library(${LIB_NAME} SHARED ${SRCS} ${HDRS} )
target_include_directories(${LIB_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(${LIB_NAME} PUBLIC PlatoFunctionalCriteria)

install( TARGETS ${LIB_NAME} EXPORT PlatoEngine
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
target_include_directories(${LIB_NAME} INTERFACE $<INSTALL_INTERFACE:include>)
install( FILES ${HDRS} DESTINATION include)

# Build unit tester
set(SRCS Test_MassObjective.cpp
         Test_ObjectiveFactory.cpp
         Test_SharedLibObjective.cpp
         UnitMain.cpp )

set(HDRS )

set(TEST_EXE PlatoFunctional_IntegrationUnitTester)
add_executable(${TEST_EXE} ${SRCS} ${HDRS})
target_link_libraries( ${TEST_EXE} PRIVATE 
    GTest::GTest TeuchosCore::teuchoscore ROL::rol PlatoFunctional PlatoTestMassObjective PlatoFunctionalTestUtilities)

add_test(NAME ${TEST_EXE} COMMAND ${TEST_EXE})
set_property(TEST ${TEST_EXE} PROPERTY LABELS "small")

install( TARGETS ${TEST_EXE} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
