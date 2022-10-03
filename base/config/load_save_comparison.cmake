message("Running the command:")
message("${TEST_COMMAND_SAVE}")
execute_process(COMMAND bash "-c" "echo ${TEST_COMMAND_SAVE}" RESULT_VARIABLE HAD_ERROR)
execute_process(COMMAND bash "-c" "${TEST_COMMAND_SAVE}" RESULT_VARIABLE HAD_ERROR)
execute_process(COMMAND bash "-c" "mv ${RESULT_FILE} ${RESULT_FILE_SAVE}" RESULT_VARIABLE HAD_ERROR)
execute_process(COMMAND bash "-c" "echo ${TEST_COMMAND_LOAD}" RESULT_VARIABLE HAD_ERROR)
execute_process(COMMAND bash "-c" "${TEST_COMMAND_LOAD}" RESULT_VARIABLE HAD_ERROR)

if(HAD_ERROR)
  message(FATAL_ERROR "run failed")
endif()

set(DIFF_TEST "diff ${RESULT_FILE} ${RESULT_FILE_SAVE}")

message("Running unix-diff command:")
message("${DIFF_TEST}")

execute_process(COMMAND bash "-c" "${DIFF_TEST}" RESULT_VARIABLE HAD_ERROR)

if(HAD_ERROR)
  message(FATAL_ERROR "Test failed")
endif()
