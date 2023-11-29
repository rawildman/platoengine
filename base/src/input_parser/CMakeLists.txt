set(SRCS Plato_FileList.cpp
         Plato_InputEnumTypes.cpp)

set(HDRS Plato_BlockStructRule.hpp
         Plato_EnumTypeHelpers.hpp
         Plato_InputBlockStruct.hpp
         Plato_InputBlocks.hpp
         Plato_EnumParser.hpp
         Plato_InputEnumTypes.hpp
         Plato_InputParser.hpp )

add_library(PlatoInputParser ${SRCS} ${HDRS})
target_link_libraries(PlatoInputParser PUBLIC PlatoParser Boost::headers)
target_include_directories(PlatoInputParser PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

if(CMAKE_CUDA_COMPILER_ID STREQUAL "NVIDIA")
  # Suppress warnings from Boost
  message(STATUS "Supressing nvcc warnings for parser")
  target_compile_options(PlatoInputParser PUBLIC 
    $<$<COMPILE_LANGUAGE:CXX>:SHELL:-Xcudafe --diag_suppress=20011 -Xcudafe --diag_suppress=20012>) 
endif()

install( TARGETS PlatoInputParser EXPORT PlatoEngine
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib )

target_include_directories(PlatoInputParser PUBLIC $<INSTALL_INTERFACE:include>)
install( FILES ${HDRS} DESTINATION include )

if( UNIT_TESTING )
  add_subdirectory(./unittest)
endif()
