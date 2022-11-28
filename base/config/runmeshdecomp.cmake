macro( runmeshdecomp SEACAS_DECOMP NUM_PROCS INPUT_MESH )

# Check for RESTART_MESH
set(OptionalArgs ${ARGN})
list(LENGTH OptionalArgs NumOptionalArgs)

if(NumOptionalArgs GREATER 0)
    list(GET OptionalArgs 0 RESTART_MESH)
endif()

string(REPLACE " " ";" NUM_PROCS_LIST ${NUM_PROCS})

message(STATUS "decomp is ${SEACAS_DECOMP}")

if (NOT SEACAS_DECOMP)
  message(FATAL_ERROR "Cannot find decomp")
endif()

if (RESTART_MESH)
  foreach(NUM_PROC ${NUM_PROCS_LIST})
    SET(DECOMP_COMMAND ${SEACAS_DECOMP} -processors ${NUM_PROC} ${RESTART_MESH})
    message("Running the command:")
    message("${DECOMP_COMMAND}")
    EXECUTE_PROCESS(COMMAND ${DECOMP_COMMAND} RESULT_VARIABLE HAD_ERROR)
  endforeach()
endif()

if(NOT ${INPUT_MESH} STREQUAL SKIP_DECOMP)
  foreach(NUM_PROC ${NUM_PROCS_LIST})
    SET(DECOMP_COMMAND ${SEACAS_DECOMP} -processors ${NUM_PROC} ${INPUT_MESH})
    message("Running the command:")
    message("${DECOMP_COMMAND}")
    EXECUTE_PROCESS(COMMAND ${DECOMP_COMMAND} RESULT_VARIABLE HAD_ERROR)
  endforeach()

  if(HAD_ERROR)
    message(FATAL_ERROR "decomp failed")
  endif()
endif()

endmacro(runmeshdecomp)