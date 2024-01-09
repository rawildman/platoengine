set(SRCS OptimizationProblem.cpp
         Main.cpp )

set(HDRS OptimizationProblem.hpp)

set(EXE_NAME plato)
add_executable(${EXE_NAME} ${SRCS} ${HDRS})
target_link_libraries(${EXE_NAME} PRIVATE ROL::rol PlatoFunctional PlatoFunctionalCore PlatoInputParser)

install( TARGETS ${EXE_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
