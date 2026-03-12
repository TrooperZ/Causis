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

add_executable(parser_debug
  tests/parser_debug.cpp
)

target_link_libraries(parser_debug PRIVATE causis_core)
