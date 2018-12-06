cmake_minimum_required(VERSION 3.6)
project(feather)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH ${PROJECT_SOURCEE_DIR}/packages)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
# Configure the main project by adding all the directories

include_directories(${PROJECT_SOURCE_DIR})
file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/**/*.cpp)
file(GLOB HEADER_FILES ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/include/**/*.h
        ${PROJECT_SOURCE_DIR}/include/**/*.hpp)
add_library(feather ${SOURCE_FILES} include/nodes/document.h include/nodes/character_data.h include/nodes/interfaces/child_node.h
        include/nodes/interfaces/parent_node.h include/nodes/interfaces/non_document_type_child_node.h src/nodes/character_data.cpp
        include/nodes/interfaces/slotable.h include/nodes/text.h src/nodes/text.cpp include/nodes/comment.h
        include/nodes/c_data_section.h include/nodes/processing_instruction.h include/utils/named_node_map.h
        include/utils/selectors/css_selector.h include/utils/selectors/css_selector_relation.h include/utils/selectors/css_selector_token.h
        include/utils/selectors/css_selector_token_group.h src/utils/selectors/css_selector.cpp src/utils/selectors/css_selector_token.cpp
        src/utils/selectors/css_selector_token_group.cpp include/utils/css_rule_list.h include/utils/css_style_declaration.h
        src/utils/selectors/css_selector_parsing_impl.cpp include/utils/selectors/css_selector_parsing_impl.h include/css/pseudoclass_manager.h
        include/nodes/shadow_root.h include/nodes/document_or_shadow_root.h include/geom/dom_rect.h include/utils/observable/observable_list.h
        include/utils/observable/filtered_list.h include/utils/observable/invalidatable.h include/utils/observable/smart_observable_list.h
        include/utils/node_list.h include/utils/html_collection.h src/utils/html_collection.cpp include/css/css_style_sheet.h
        include/css/style_sheet.h include/css/css_rules/css_rule.h include/css/css_rules/css_rule_type.h include/css/css_rules/css_import_rule.h
        include/utils/observable/observable_map.h include/utils/observable/smart_observable_map.h include/utils/observable/dumb_observable_list.h
        include/nodes/attrs/standard_attr.h include/nodes/attrs/class_attr.h include/utils/observable/concatenated_list.h
        include/utils/observable/event/event_type.h include/typedefs.h src/typedefs.cpp include/utils/full_child_list.h src/utils/full_child_list.cpp include/utils/observable/mutable_filtered_list.h src/css/pseudoclass_manager.cpp)
target_include_directories(feather PRIVATE include)

add_executable(render-engine ${PROJECT_SOURCE_DIR}/src/main.cpp src/typedefs.cpp)
target_include_directories(render-engine PRIVATE include)
target_link_libraries(render-engine feather)
find_package(curl)


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