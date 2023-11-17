set(SRCS TwoDTestTypes.cpp
         UnitMain.cpp)

set(HDRS Penalty.hpp
         Rosenbrock.hpp
         TwoDTestTypes.hpp
         UnitMain.hpp)

set(LIB_NAME PlatoFunctionalTestUtilities)
add_library(${LIB_NAME} ${SRCS} ${HDRS} )
target_include_directories(${LIB_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> )
target_link_libraries(${LIB_NAME} PRIVATE Kokkos::kokkos GTest::GTest)

install( TARGETS ${LIB_NAME} EXPORT PlatoEngine
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
target_include_directories(${LIB_NAME} INTERFACE $<INSTALL_INTERFACE:include>)
install( FILES ${HDRS} DESTINATION include)
