cmake_minimum_required(VERSION 3.6)
project(feather)
set(CMAKE_CXX_STANDARD 17)


include_directories(${PROJECT_SOURCE_DIR})
file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/**/*.cpp ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(feather ${SOURCE_FILES} include/events/event_target.h include/events/event.h include/typedefs.h include/events/event_phase.h src/events/event.cpp src/events/event_target.cpp)
add_executable(render-engine ${PROJECT_SOURCE_DIR}/src/main.cpp)
find_package(curl)