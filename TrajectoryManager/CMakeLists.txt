cmake_minimum_required(VERSION 3.16)

project(TrajectoryManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(TrajectoryManager STATIC
  trajectorymanager.cpp
  trajectorymanager.h
)

target_compile_definitions(TrajectoryManager PRIVATE TRAJECTORYMANAGER_LIBRARY)


target_link_libraries(TrajectoryManager
    PUBLIC Qt6::Core
    Qt6::Positioning
    libtle
    libbase
)

target_include_directories(TrajectoryManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/../libtle
    ${PROJECT_SOURCE_DIR}/../libbase
)

