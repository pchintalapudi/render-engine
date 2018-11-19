cmake_minimum_required(VERSION 3.6)
project(feather)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH ${PROJECT_SOURCEE_DIR}/packages)

# Configure the main project by adding all the directories

include_directories(${PROJECT_SOURCE_DIR})
file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/**/*.cpp ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(feather ${SOURCE_FILES} include/events/event_target.h include/events/event.h include/typedefs.h include/events/event_phase.h src/events/event.cpp src/events/event_target.cpp include/nodes/node.h include/nodes/node_type.h src/nodes/node.cpp include/nodes/element.h include/nodes/document.h include/nodes/character_data.h include/nodes/interfaces/child_node.h include/nodes/interfaces/parent_node.h include/nodes/interfaces/non_document_type_child_node.h src/nodes/character_data.cpp include/nodes/interfaces/slotable.h include/nodes/text.h src/nodes/text.cpp include/nodes/comment.h include/nodes/c_data_section.h include/nodes/processing_instruction.h include/utils/named_node_map.h include/utils/html_collection.h include/utils/node_list.h src/utils/node_list.cpp include/nodes/attr.h src/utils/html_collection.cpp include/utils/dom_token_list.h src/utils/dom_token_list.cpp src/nodes/element.cpp src/utils/named_node_map.cpp include/nodes/shadow_root.h include/nodes/document_or_shadow_root.h include/css/style_sheet.h include/css/css_style_sheet.h include/css/css_rules/css_rule.h include/css/css_rules/css_rule_type.h include/utils/css_rule_list.h include/css/css_rules/css_import_rule.h include/css/css_rules/css_style_rule.h include/css/css_rules/css_media_rule.h include/css/css_rules/css_font_face_rule.h include/css/css_rules/css_page_rule.h include/css/css_rules/css_keyframes_rule.h include/css/css_rules/css_keyframe_rule.h include/css/css_rules/css_namespace_rule.h include/css/css_rules/css_counter_style_rule.h include/css/css_rules/css_supports_rule.h include/css/css_rules/css_document_rule.h include/css/css_rules/css_font_features_value_rule.h include/css/css_rules/css_viewport_rule.h include/css/css_rules/css_region_style_rule.h include/utils/css_style_declaration.h include/utils/selectors/css_selector.h include/utils/selectors/css_selector_token.h include/utils/selectors/css_selector_token_group.h include/utils/selectors/css_selector_relation.h src/utils/selectors/css_selector_token.cpp src/utils/selectors/css_selector_token_group.cpp src/utils/selectors/css_selector.cpp)
file(GLOB HEADER_FILES ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/include/**/*.h
        ${PROJECT_SOURCE_DIR}/include/**/*.hpp)
add_library(feather ${SOURCE_FILES} include/nodes/document.h include/nodes/character_data.h include/nodes/interfaces/child_node.h
        include/nodes/interfaces/parent_node.h include/nodes/interfaces/non_document_type_child_node.h src/nodes/character_data.cpp
        include/nodes/interfaces/slotable.h include/nodes/text.h src/nodes/text.cpp include/nodes/comment.h
        include/nodes/c_data_section.h include/nodes/processing_instruction.h include/utils/named_node_map.h
        include/utils/html_collection.h include/utils/node_list.h src/utils/node_list.cpp)
add_executable(render-engine ${PROJECT_SOURCE_DIR}/src/main.cpp)
find_package(curl)

target_include_directories(feather PRIVATE include)


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
gtest_discover_tests(parser_tests)
