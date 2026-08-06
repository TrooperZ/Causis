
if(NOT DEFINED CAUSIS OR NOT EXISTS "${CAUSIS}")
  message(FATAL_ERROR "Missing causis executable: ${CAUSIS}")
endif()

if(NOT DEFINED SOURCE OR NOT EXISTS "${SOURCE}")
  message(FATAL_ERROR "Missing test source: ${SOURCE}")
endif()

get_filename_component(CASE_DIRECTORY "${SOURCE}" DIRECTORY)
get_filename_component(CASE_NAME "${SOURCE}" NAME_WE)

set(EXPECTED_OUT_FILE "${CASE_DIRECTORY}/${CASE_NAME}.out")
set(EXPECTED_ERR_FILE "${CASE_DIRECTORY}/${CASE_NAME}.err")

if(NOT EXISTS "${EXPECTED_OUT_FILE}" AND NOT EXISTS "${EXPECTED_ERR_FILE}")
  message(FATAL_ERROR "${CASE_NAME}: missing .out or .err file")
endif()

set(EXPECTED_OUT "")
set(EXPECTED_ERR "")
set(EXPECTED_EXIT 0)

if(EXISTS "${EXPECTED_OUT_FILE}")
  file(READ "${EXPECTED_OUT_FILE}" EXPECTED_OUT)
endif()

if(EXISTS "${EXPECTED_ERR_FILE}")
  file(READ "${EXPECTED_ERR_FILE}" EXPECTED_ERR)
  set(EXPECTED_EXIT 1)
endif()

execute_process(
  COMMAND "${CAUSIS}" "${SOURCE}"
  RESULT_VARIABLE ACTUAL_EXIT
  OUTPUT_VARIABLE ACTUAL_OUT
  ERROR_VARIABLE ACTUAL_ERR
  TIMEOUT 5
)

if(NOT "${ACTUAL_EXIT}" STREQUAL "${EXPECTED_EXIT}")
  message(FATAL_ERROR
    "${CASE_NAME}: expected exit ${EXPECTED_EXIT}, got ${ACTUAL_EXIT}"
  )
endif()

if(NOT "${ACTUAL_OUT}" STREQUAL "${EXPECTED_OUT}")
  message(FATAL_ERROR
    "${CASE_NAME}: stdout mismatch\n"
    "EXPECTED:\n${EXPECTED_OUT}\n"
    "ACTUAL:\n${ACTUAL_OUT}"
  )
endif()

if(NOT "${ACTUAL_ERR}" STREQUAL "${EXPECTED_ERR}")
  message(FATAL_ERROR
    "${CASE_NAME}: stderr mismatch\n"
    "EXPECTED:\n${EXPECTED_ERR}\n"
    "ACTUAL:\n${ACTUAL_ERR}"
  )
endif()