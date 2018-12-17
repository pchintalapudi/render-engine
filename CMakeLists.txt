cmake_minimum_required(VERSION 3.6)
project(feather)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH ${PROJECT_SOURCEE_DIR}/packages)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

include(external/CMakeLists.txt)

# Configure the main project by adding all the directories
add_subdirectory(src)
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/**/*.cpp)
file(GLOB HEADER_FILES ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/include/**/*.h
        ${PROJECT_SOURCE_DIR}/include/**/*.hpp)
add_library(feather ${SOURCE_FILES}
        include/typedefs.h include/observable/invalidatable.h src/observable/invalidatable.cpp include/observable/observable_list.h include/observable/invalidation_event.h include/enums/enum_set.h include/enums/enum_map.h include/events/event_target.h include/events/event.h src/events/event.cpp src/events/event_target.cpp include/nodes/node.h include/nodes/documents/document.h include/nodes/elements/element.h include/observable/observable_item.h include/nodes/text/character_data.h include/nodes/text/text.h include/nodes/slotable.h src/nodes/node.cpp src/nodes/text/text.cpp include/nodes/text/comment.h include/nodes/elements/html_elements/html_element.h include/nodes/elements/html_elements/html_html_element.h include/nodes/text/c_data_section.h include/nodes/text/processing_instruction.h include/nodes/elements/selectors/css_selector.h include/nodes/elements/selectors/xpath_selector.h include/nodes/utils/dom_token_list.h src/nodes/utils/dom_token_list.cpp include/nodes/utils/named_node_map.h include/nodes/utils/attr.h src/nodes/utils/named_node_map.cpp include/nodes/documents/shadow_root.h include/nodes/utils/html_collection.h src/nodes/utils/html_collection.cpp include/geom/dom_rect.h src/nodes/elements/element.cpp include/style/css/pseudoclass_manager.h src/css/pseudoclass_manager.cpp src/nodes/elements/selectors/css_selector.cpp)
target_include_directories(feather PRIVATE include)

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