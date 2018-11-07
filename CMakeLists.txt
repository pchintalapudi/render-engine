cmake_minimum_required(VERSION 3.6)
project(feather)
set(CMAKE_CXX_STANDARD 11)


include_directories(${PROJECT_SOURCE_DIR})
file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/**/*.cpp ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(feather ${SOURCE_FILES} include/visuals/vnode.h include/visuals/text.h include/visuals/html_element.h include/visuals/NodeType.h)
find_package(curl)