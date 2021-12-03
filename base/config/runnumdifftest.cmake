# 1. Run the program and generate the exodus output

message("Running the command:")
message("${TEST_COMMAND}")
execute_process(COMMAND bash "-c" "echo ${TEST_COMMAND}" RESULT_VARIABLE HAD_ERROR)
execute_process(COMMAND bash "-c" "${TEST_COMMAND}" RESULT_VARIABLE HAD_ERROR)

if(HAD_ERROR)
  message(FATAL_ERROR "run failed")
endif()

# 2. Run numdiff command
SET(DIFF_TEST "${NUMDIFF_COMMAND} -r ${NUMDIFF_RELATIVE_TOLERANCE} ${OUT_FILE} ${DATA_DIR}/${GOLD_FILE}")

message("Running unix-diff command:")
message("${DIFF_TEST}")

#EXECUTE_PROCESS(COMMAND ${DIFF_TEST} RESULT_VARIABLE HAD_ERROR)
execute_process(COMMAND bash "-c" "${DIFF_TEST}" RESULT_VARIABLE HAD_ERROR)

if(HAD_ERROR)
  message(FATAL_ERROR "Test failed")
endif()
