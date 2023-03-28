SET(PlatoPython_SRCS PlatoPython_main.cpp 
                     PlatoPythonApp.cpp
                     PythonAppUtilities.cpp
                     PythonOperationFactory.cpp
                     PlatoPythonOperation.cpp
                     CriterionValueOperation.cpp
                     CriterionGradientOperation.cpp
                     RunVoidFunctionOperation.cpp)

SET(PlatoPython_HDRS PlatoPythonApp.hpp
                     PythonAppUtilities.hpp
                     PythonOperationFactory.hpp
                     PlatoPythonOperation.hpp
                     CriterionValueOperation.hpp
                     CriterionGradientOperation.hpp
                     CriterionValueAndGradientOperation.hpp
                     RunVoidFunctionOperation.hpp)

include_directories(${PLATO_INCLUDES})

# actual target:
set(PlatoPython_LIBS PlatoInterface 
                     PlatoParser 
                     PlatoExceptions)

find_package(Boost REQUIRED COMPONENTS python)
find_package(Python COMPONENTS Interpreter Development REQUIRED)

add_executable(PythonInterpreter ${PlatoPython_SRCS})
target_link_libraries(PythonInterpreter 
                      ${PlatoPython_LIBS}
                      Python::Python
                      Boost::python)

set(PlatoPython_BINARY ${CMAKE_BINARY_DIR}/apps/python_interpreter/PythonInterpreter PARENT_SCOPE)

if( CMAKE_INSTALL_PREFIX )
  install( TARGETS PythonInterpreter DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
endif()

if( UNIT_TESTING )
  add_subdirectory(./unittest)
endif()
