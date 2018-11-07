cmake_minimum_required(VERSION 3.6)
project(feather)
set(CMAKE_CXX_STANDARD 17)


include_directories(${PROJECT_SOURCE_DIR})
file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/**/*.cpp ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(feather ${SOURCE_FILES} include/nodes/inode.h include/nodes/text.h include/nodes/ielement.h include/nodes/node_type.h include/nodes/idocument.h include/nodes/utils/attr.h include/nodes/utils/named_node_map.h include/nodes/utils/dom_token_list.h include/nodes/icharacter_data.h include/nodes/utils/typedefs.h include/events/ievent_target.h include/events/event.h src/nodes/inode.cpp include/nodes/ichild_node.h include/nodes/inon_document_type_child_node.h)
find_package(curl)