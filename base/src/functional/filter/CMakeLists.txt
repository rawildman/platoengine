set(SRCS library/FilterFactory.cpp
         library/FilterJacobian.cpp
         library/FilterRegistration.cpp
         library/FilterValidation.cpp
         extension/HelmholtzFilter.cpp
         extension/IdentityFilter.cpp)

set(HDRS library/FilterInterface.hpp
         library/FilterFactory.hpp
         library/FilterJacobian.hpp
         library/FilterRegistration.hpp
         library/FilterValidation.hpp
         extension/HelmholtzFilter.hpp
         extension/IdentityFilter.hpp)

set(LIB_NAME PlatoFunctionalFilter)
add_library(${LIB_NAME} SHARED ${SRCS} ${HDRS} )
target_include_directories(${LIB_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/library>)
target_include_directories(${LIB_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/extension>)

target_link_libraries(${LIB_NAME} PUBLIC PlatoFunctionalCore PlatoFunctionalUtilities PlatoInputParser)

install( TARGETS ${LIB_NAME} EXPORT PlatoEngine
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
target_include_directories(${LIB_NAME} INTERFACE $<INSTALL_INTERFACE:include>)
install( FILES ${HDRS} DESTINATION include)

if( UNIT_TESTING )
  # Build test filter shared lib
  set(SRCS testutilities/SharedLibIdentityFilter.cpp)
  set(HDRS testutilities/SharedLibIdentityFilter.hpp)

  set(LIB_NAME PlatoIdentityFilter)
  add_library(${LIB_NAME} SHARED ${SRCS} ${HDS} )
  target_include_directories(${LIB_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/testutilities>)
  target_link_libraries(${LIB_NAME} PUBLIC PlatoFunctionalFilter)

  install( TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

  # Build unit tester
  set(SRCS library/unittest/Test_FilterFactory.cpp
           library/unittest/Test_FilterValidation.cpp
           library/unittest/Test_Registration.cpp
           library/unittest/Test_SharedLibFilter.cpp
           extension/unittest/Test_HelmholtzFilter.cpp
           extension/unittest/Test_IdentityFilter.cpp
           testutilities/UnitMain.cpp )

  set(HDRS )

  set(TEST_EXE PlatoFunctional_FilterUnitTester )
  add_executable(${TEST_EXE} ${SRCS} ${HDRS})
  target_include_directories(${TEST_EXE} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/testutilities>)
  target_link_libraries( ${TEST_EXE} PRIVATE GTest::GTest PlatoFunctionalFilter PlatoFunctionalTestUtilities)

  add_test(NAME ${TEST_EXE} COMMAND ${TEST_EXE})
  set_property(TEST ${TEST_EXE} PROPERTY LABELS "small")

  install( TARGETS ${TEST_EXE} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

endif()