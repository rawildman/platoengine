# 0. decompose mesh if parallel

include(${SOURCE_DIR}/base/config/runmeshdecomp.cmake)
runmeshdecomp(${SEACAS_DECOMP} ${NUM_PROCS} ${INPUT_MESH} ${RESTART_MESH})

# 1. Run the program and generate the exodus output
include(${SOURCE_DIR}/base/config/runnumdifftest.cmake)
