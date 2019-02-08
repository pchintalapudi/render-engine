cmake_minimum_required(VERSION 3.12)
project(feather CXX)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH ${PROJECT_SOURCEE_DIR}/packages)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

cmake_policy(SET CMP0076 NEW)

include(external/CMakeLists.txt)

# Configure the main project by adding all the directories
include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(feather)
target_include_directories(feather PUBLIC include)
add_subdirectory(src)

add_executable(render-engine ${PROJECT_SOURCE_DIR}/src/main.cpp)

target_include_directories(render-engine PRIVATE include)
target_link_libraries(render-engine feather)


configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif ()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif ()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# Now simply link against gtest or gtest_main as needed. Eg
file(GLOB PARSER_TEST_FILES ${PROJECT_SOURCE_DIR}/tests/parser/TrieTest.cpp
       ${PROJECT_SOURCE_DIR}/tests/parser/TrieTest.h)

add_executable(parser_tests ${PARSER_TEST_FILES})
target_link_libraries(parser_tests gtest gtest_main feather)
target_include_directories(parser_tests PRIVATE include)


include(GoogleTest)
add_test(NAME example_test COMMAND parser_tests)