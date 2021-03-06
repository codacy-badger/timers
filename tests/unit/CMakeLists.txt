cmake_minimum_required(VERSION 3.0)

project(tests VERSION 0.9.5 LANGUAGES CXX)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    message(FATAL_ERROR "This CMAKE is meant to be run from the top-level CMake in the root directory, run the top-level CMake instead")
endif()

include("${CMAKE_SOURCE_DIR}/cmake-helpers/gtest.cmake")
include("${CMAKE_SOURCE_DIR}/cmake-helpers/settings.cmake")
include("${CMAKE_SOURCE_DIR}/cmake-helpers/warnings.cmake")

file(GLOB_RECURSE HEADERS include/*.h*)
file(GLOB_RECURSE SOURCES src/*.c*)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

add_dependencies(${PROJECT_NAME} gtest-build)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
add_custom_target(run-all-tests-verbose COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS ${PROJECT_NAME})
