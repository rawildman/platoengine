# 0. Run XMLGenerator on plato input deck

message("Running the command:")
message("${XMLGEN_COMMAND}")
execute_process(COMMAND bash "-c" "echo ${XMLGEN_COMMAND}" RESULT_VARIABLE HAD_ERROR)
execute_process(COMMAND bash "-c" "${XMLGEN_COMMAND}" RESULT_VARIABLE HAD_ERROR)

# 1. Run the program and generate the exodus output

message("Running the command:")
message("${TEST_COMMAND}")
execute_process(COMMAND bash "-c" "echo ${TEST_COMMAND}" RESULT_VARIABLE HAD_ERROR)
execute_process(COMMAND bash "-c" "${TEST_COMMAND}" RESULT_VARIABLE HAD_ERROR)

if(HAD_ERROR)
  message(FATAL_ERROR "run failed")
endif()

# 2. Run numdiff command

message("Running custom command:")
message("${CUSTOM_COMMAND}")

execute_process(COMMAND bash "-c" "${CUSTOM_COMMAND}" RESULT_VARIABLE HAD_ERROR)

if(HAD_ERROR)
  message(FATAL_ERROR "Test failed")
endif()
