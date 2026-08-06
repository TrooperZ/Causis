cmake_minimum_required(VERSION 3.20)

project(Causis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()


add_library(causis_core SHARED
  src/Language.cpp
  src/Lexer.cpp
  src/Parser.cpp
  src/Interpreter.cpp
)

# Tell the compiler where to find headers when building causis_core
target_include_directories(
  causis_core
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include
)

# Keep the interface include for consumers of causis_core (optional)
target_include_directories(
  causis_core
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include
)


add_executable(causis
  src/main.cpp
)

target_link_libraries(causis PRIVATE causis_core)

include(CTest)

if(BUILD_TESTING)
  file(
    GLOB LANGUAGE_CASES
    CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/tests/cases/*/*.au"
  )

  foreach(CASE_FILE IN LISTS LANGUAGE_CASES)
    get_filename_component(CASE_NAME "${CASE_FILE}" NAME_WE)

    add_test(
      NAME "language.${CASE_NAME}"
      COMMAND
        "${CMAKE_COMMAND}"
        "-DCAUSIS=$<TARGET_FILE:causis>"
        "-DSOURCE=${CASE_FILE}"
        -P "${PROJECT_SOURCE_DIR}/tests/run_case.cmake"
    )
  endforeach()
endif()
