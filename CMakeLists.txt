cmake_minimum_required(VERSION 3.30.0)

set(CMAKE_CXX_STANDARD_REQUIRED 17)

project(cinitpp)
set(VERSION 2.0.0)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wfloat-equal -Werror -Wformat=2 -Wno-missing-declarations -Wmissing-include-dirs -Wcast-qual -Wpointer-arith -Wsequence-point -Wshadow -Wextra -Wcast-align -Wswitch -Wundef -Wunreachable-code -Wno-format-extra-args -Wno-unused-function -Wno-unused-parameter -Wwrite-strings -Wstrict-aliasing -Wpedantic")

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
  add_executable(${PROJECT_NAME} ${SOURCES})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
else() 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
  add_executable(${PROJECT_NAME} ${SOURCES})
endif()

target_precompile_headers(${PROJECT_NAME} PRIVATE <array> <cstdint> <iostream> <memory> <string> <utility> <variant> <vector>)

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${INCLUDE_DIR}")
