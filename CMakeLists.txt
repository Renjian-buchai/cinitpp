cmake_minimum_required(VERSION 3.30.0)


project(cinitpp)
set(VERSION 2.0.0)

find_package(nlohmann_json 3.11.0 REQUIRED)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wfloat-equal -Werror -Wformat=2 -Wno-missing-declarations -Wmissing-include-dirs -Wcast-qual -Wpointer-arith -Wsequence-point -Wshadow -Wextra -Wcast-align -Wswitch -Wundef -Wunreachable-code -Wno-format-extra-args -Wno-unused-function -Wno-unused-parameter -Wwrite-strings -Wstrict-aliasing -Wpedantic")

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
  add_executable(${PROJECT_NAME} ${SOURCES})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
else() 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
  add_executable(${PROJECT_NAME} ${SOURCES})
  add_custom_command(TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/.cinitpp.json
                ${CMAKE_CURRENT_BINARY_DIR}/.cinitpp.json)
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED 17)

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${INCLUDE_DIR}")

target_include_directories(${PROJECT_NAME} PRIVATE "U:/msys64/ucrt64/include")

target_precompile_headers(${PROJECT_NAME} PRIVATE <array> <cstdint> <iostream> <memory> <string> <utility> <variant> <vector> <nlohmann/json.hpp>)
