###############################################################################
# Sources:
###############################################################################
SET(PlatoXTK_UnitTester_SRCS UnitMainXTK.cpp
                             Plato_Test_XTK_Application.cpp)

SET(PlatoXTK_UnitTester_HDRS )

set(PLATOUNIT_INCLUDES ${CMAKE_SOURCE_DIR}/apps/xtk
                       ${CMAKE_SOURCE_DIR}/base/src/tools)

foreach(MRSLIB  ${ALL_MRS_LIBS})
    get_target_property(MRSLIB_INCLUDE ${MRSLIB} INTERFACE_INCLUDE_DIRECTORIES)
    string(APPEND PLATOUNIT_INCLUDES "${MRSLIB_INCLUDE};")
endforeach()

SET( testFiles "xtkApp.xml;xtkInput.xml;ut_xtk_bolted_bracket.exo" )
Plato_add_test_files("${testFiles}")

INCLUDE_DIRECTORIES(${PLATOUNIT_INCLUDES})
set(PlatoXTK_UnitTester_LIBS 
    ${GTEST_LIBRARY} 
    ${PLATO_LIBRARIES} 
    ${PLATO_LIBRARIES} # NOTE: PLATO_LIBRARIES needs to be linked against twice
    ${PETSc_Libs}
    ${XTK_LIBRARIES}
    ${PLATOXTK}
    ${Trilinos_LIBRARIES} 
    ${Trilinos_TPL_LIBRARIES}
    ${Plato_EXTRA_LINK_FLAGS} )

add_executable(PlatoXTK_UnitTester ${PlatoXTK_UnitTester_SRCS})
target_link_libraries(PlatoXTK_UnitTester ${PlatoXTK_UnitTester_LIBS})
add_test(NAME PlatoXTK_UnitTester COMMAND ${CMAKE_BINARY_DIR}/apps/xtk/unittest/PlatoXTK_UnitTester)

if( CMAKE_INSTALL_PREFIX )
  install( TARGETS PlatoXTK_UnitTester DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
endif()
