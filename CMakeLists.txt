cmake_minimum_required(VERSION 3.10)
project(Diagnostics VERSION 0.1 LANGUAGES CXX)
set(DIAGNOSTICS_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${DIAGNOSTICS_CMAKE_PATH}")
set(DIAGNOSTICS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(DIAGNOSTICS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

include(TargetWarnings)

option(DIAGNOSTICS_BUILD_UNIT_TESTS "build unit tests")
add_subdirectory(src)

if (DIAGNOSTICS_BUILD_UNIT_TESTS)
  add_subdirectory(test)
endif()
