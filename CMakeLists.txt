cmake_minimum_required(VERSION 3.20)

project(Causis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(causis_core INTERFACE)
target_include_directories(
  causis_core
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(causis
  src/main.cpp
)

target_link_libraries(causis PRIVATE causis_core)
