# Build test filter shared lib
set(SRCS SharedLibIdentityFilter.cpp)
set(HDRS SharedLibIdentityFilter.hpp)

set(LIB_NAME PlatoIdentityFilter)
add_library(${LIB_NAME} SHARED ${SRCS} ${HDS} )
target_include_directories(${LIB_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(${LIB_NAME} PUBLIC PlatoFunctionalFilter)

install( TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

# Build unit tester
set(SRCS Test_FilterFactory.cpp
         Test_FilterValidation.cpp
         Test_IdentityFilter.cpp
         Test_Registration.cpp
         Test_SharedLibFilter.cpp
         UnitMain.cpp )

set(HDRS )

set(TEST_EXE PlatoFunctional_FilterUnitTester )
add_executable(${TEST_EXE} ${SRCS} ${HDRS})
target_link_libraries( ${TEST_EXE} PRIVATE GTest::GTest PlatoFunctionalFilter PlatoFunctionalTestUtilities)

add_test(NAME ${TEST_EXE} COMMAND ${TEST_EXE})
set_property(TEST ${TEST_EXE} PROPERTY LABELS "small")

install( TARGETS ${TEST_EXE} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
