cmake_minimum_required(VERSION 3.6)
project(feather)
set(CMAKE_CXX_STANDARD 17)


include_directories(${PROJECT_SOURCE_DIR})
file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/**/*.cpp ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(feather ${SOURCE_FILES} include/events/event_target.h include/events/event.h include/typedefs.h include/events/event_phase.h src/events/event.cpp src/events/event_target.cpp include/nodes/node.h include/nodes/node_type.h src/nodes/node.cpp include/nodes/element.h include/nodes/document.h include/nodes/character_data.h include/nodes/interfaces/child_node.h include/nodes/interfaces/parent_node.h include/nodes/interfaces/non_document_type_child_node.h src/nodes/character_data.cpp include/nodes/interfaces/slotable.h include/nodes/text.h src/nodes/text.cpp include/nodes/comment.h include/nodes/c_data_section.h include/nodes/processing_instruction.h)
add_executable(render-engine ${PROJECT_SOURCE_DIR}/src/main.cpp)
find_package(curl)