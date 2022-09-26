
# 1. Run the program and generate the exodus output

message("Running the command:")
message("${TEST_COMMAND}")
execute_process(COMMAND bash "-c" "echo ${TEST_COMMAND}" RESULT_VARIABLE HAD_ERROR)
execute_process(COMMAND bash "-c" "${TEST_COMMAND}" RESULT_VARIABLE HAD_ERROR)

# 2. Check for failure string

execute_process(COMMAND bash "-c" "grep '${EXPECTED_STRING}' ${OUTPUT_FILE}" RESULT_VARIABLE HAD_ERROR)

if(HAD_ERROR)
  message(FATAL_ERROR "Test failed")
endif()
