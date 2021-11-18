
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")

SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/ALG/" ${CMAKE_PREFIX_PATH})    
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/COM/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/FEM/INT/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/FEM/MDL/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/FEM/MSI/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/FEM/VIS/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/COM/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/GEN/GEN_CORE/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/GEN/GEN_MAIN/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/moris/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/moris/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/MRS/ASR/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/MRS/CHR/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/MRS/CON/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/MRS/COR/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/MRS/EXC/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/MRS/IOS/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/MTK/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/MTK/MAP" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/OPT/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/PRM/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/SOL/DLA/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/SOL/NLA/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/SOL/SOL_CORE/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/SOL/TSA/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/TOL/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/WRK/" ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH "$ENV{XTK_INSTALL}/share/XTK/" ${CMAKE_PREFIX_PATH})

find_package( COM REQUIRED HINTS $ENV{XTK_INSTALL}/share/COM/)
find_package( MTK REQUIRED )
find_package( INT REQUIRED )
find_package( MSI REQUIRED )
find_package( VIS REQUIRED )
find_package( XTK REQUIRED )
find_package( GEN_CORE REQUIRED )
find_package( GEN_MAIN REQUIRED )
find_package( PRM REQUIRED )

# find_package(PETSc)
set(moris_DEFINITIONS "-DF77ADD_;-DMORIS_PERFORM_CHECK;-DUSE_XTK;-DXTK_USE_MORIS;-DMORIS_USE_32BIT;-DMORIS_USE_ARMA;-DARMA_NO_DEBUG;-DDEBUG")

add_definitions(${moris_DEFINITIONS})

set(XTK_Libs)
list(APPEND	XTK_Libs /home/doble/Codes/spack_plato/install/moris/lib/XTK/libXTK.a )

set(LIB_NAMES)
set(LIB_NAME XTK)
set(LIB_NAMES ${LIB_NAMES} ${LIB_NAME})

###############################################################################
# Sources:
###############################################################################
set(XTKMAIN_SOURCES  Plato_XTK_Application.cpp 
                     Plato_XTK_Local_Op_Compute_Bounds.cpp
					 Plato_XTK_Local_Op_Generate_Model.cpp
					 Plato_XTK_Local_Op_Sensitivity.cpp
					 Plato_XTK_Local_Op_Load_From_Hdf5.cpp
					 Plato_XTK_Local_Op_Save_To_Hdf5.cpp)
				     
set(XTKMAIN_HEADERS  Plato_XTK_Application.hpp
					 Plato_XTK_SharedData.hpp
					 Plato_XTK_Local_Op.hpp
                     Plato_XTK_Local_Op_Compute_Bounds.hpp
					 Plato_XTK_Local_Op_Generate_Model.hpp
					 Plato_XTK_Local_Op_Sensitivity.hpp
					 Plato_XTK_Local_Op_Load_From_Hdf5.hpp
					 Plato_XTK_Local_Op_Save_To_Hdf5.hpp
					 Plato_XTK_Local_Op_Generate_Model.hpp
					 )

include_directories(SYSTEM ${PLATO_INCLUDES} ${XTK_INCLUDES})

# actual target:
set(XTKMAIN_LIBS  
    ${PLATO_LIBRARIES} 
    ${PLATO_LIBRARIES} # NOTE: PLATO_LIBRARIES needs to be linked against twice
    ${PETSc_Libs}
    ${MTK_LIBRARIES}
    ${ALL_MRS_LIBS}
    ${ALL_MRS_LIBS}
    ${Trilinos_LIBRARIES} 
    ${Trilinos_TPL_LIBRARIES}
    ${Plato_EXTRA_LINK_FLAGS})


set(MORIS_INCLUDES)
foreach(MRSLIB  ${ALL_MRS_LIBS})
    get_target_property(MRSLIB_INCLUDE ${MRSLIB} INTERFACE_INCLUDE_DIRECTORIES)
    string(APPEND MORIS_INCLUDES "${MRSLIB_INCLUDE};")
endforeach()

# MPMD executable
add_executable(XTKMain ${XTKMAIN_SOURCES} XTK_Main.cpp)
set_target_properties(XTKMain PROPERTIES NO_SYSTEM_FROM_IMPORTED true )
target_link_libraries(XTKMain ${XTKMAIN_LIBS})
add_library(PlatoXTK     ${XTKMAIN_SOURCES}     ${XTKMAIN_HEADERS}     )
target_link_libraries(XTKMain ${XTKMAIN_LIBS})
target_include_directories(PlatoXTK PUBLIC ${MORIS_INCLUDES})

add_executable(XTK ${XTKMAIN_SOURCES} XTK_Main_Standalone.cpp)
target_link_libraries(XTK ${XTKMAIN_LIBS})

# add PlatoXTK to plato libraries
set(PLATO_LIBRARIES ${PLATO_LIBRARIES} PlatoXTK)
set(XTKMAIN_BINARY ${CMAKE_BINARY_DIR}/apps/xtk/XTKMain PARENT_SCOPE)
set(XTK_BINARY ${CMAKE_BINARY_DIR}/apps/xtk/XTK PARENT_SCOPE)

if( CMAKE_INSTALL_PREFIX )
  install( TARGETS XTKMain DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
endif()


if( UNIT_TESTING )
  add_subdirectory(./unittest)
endif()
