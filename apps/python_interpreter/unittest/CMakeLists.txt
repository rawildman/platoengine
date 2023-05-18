SET(PlatoPythonAppUnitTester_SRCS UnitMain.cpp
                                  ../PlatoPythonApp.cpp
                                  ../PythonAppUtilities.cpp
                                  ../PythonOperationFactory.cpp
                                  ../PlatoPythonOperation.cpp
                                  ../CriterionValueOperation.cpp
                                  ../CriterionGradientOperation.cpp
                                  ../RunVoidFunctionOperation.cpp
	                                Plato_Test_PythonApp.cpp)

SET(PlatoPythonAppUnitTester_HDRS )

set(PLATOUNIT_INCLUDES ${CMAKE_SOURCE_DIR}/apps/python_interpreter)
include_directories(${PLATOUNIT_INCLUDES})

set(PlatoPythonAppUnitTester_LIBS GTest::GTest 
                                  PlatoParser 
                                  PlatoDataLayer 
                                  PlatoExceptions)

find_package(Boost REQUIRED COMPONENTS python)
find_package(Python COMPONENTS Interpreter Development REQUIRED)

add_executable(PythonAppUnitTester ${PlatoPythonAppUnitTester_SRCS})
target_link_libraries(PythonAppUnitTester 
                      ${PlatoPythonAppUnitTester_LIBS}
                      Python::Python
                      Boost::python)

add_test(NAME PythonAppUnitTester COMMAND ${CMAKE_BINARY_DIR}/apps/python_interpreter/unittest/PythonAppUnitTester)
set_property(TEST PythonAppUnitTester PROPERTY LABELS "small")

configure_file(  ${CMAKE_SOURCE_DIR}/apps/python_interpreter/unittest/test_functions.py
                 ${CMAKE_BINARY_DIR}/apps/python_interpreter/unittest/test_functions.py COPYONLY)

configure_file(  ${CMAKE_SOURCE_DIR}/apps/python_interpreter/unittest/import_functions.py
                 ${CMAKE_BINARY_DIR}/apps/python_interpreter/unittest/import/import_functions.py COPYONLY)

configure_file(  ${CMAKE_SOURCE_DIR}/apps/python_interpreter/unittest/numpy_functions.py
                 ${CMAKE_BINARY_DIR}/apps/python_interpreter/unittest/import/numpy_functions.py COPYONLY)

if( CMAKE_INSTALL_PREFIX )
  install( TARGETS PythonAppUnitTester DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
  file( GLOB PYTHON_FILES *.py )
  install( FILES ${PYTHON_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
endif()
